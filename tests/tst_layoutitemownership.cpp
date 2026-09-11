#include <QTest>
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>
#include <QWidget>

#include <atomic>
#include <cstdlib>
#include <new>

#include <Kanoop/gui/widgets/buttonlabel.h>
#include <Kanoop/gui/widgets/flowlayout.h>

/*
 * What this file covers, and what it does not.
 *
 * QLayout::takeAt transfers ownership of the returned QLayoutItem to the caller.
 * Dropping it leaks. The layout's own item count is back to normal either way, so
 * the leak has no observable surface in the widget API.
 *
 * Rather than depend on valgrind, which cannot be a ctest dependency, this file
 * replaces the global allocation operators and counts live blocks.
 *
 * That counts the allocation under test only where the replacement in this
 * executable preempts the one inside libQt6Widgets, which is what ELF symbol
 * interposition gives on Linux. A platform that binds each module to its own
 * allocator, as Windows does by giving every DLL its own CRT heap, leaves
 * QLayoutPrivate::createWidgetItem's allocation invisible here, and both leak
 * assertions below would then hold for any implementation. That is not a
 * hypothetical to be guarded with an #ifdef: counter_seesAllocationsMadeInsideQt
 * measures it, and the two leak cases refuse to run when it does not hold.
 *
 * NOT covered here: the secondary effect of the leak, where the orphaned item stays
 * registered as the widget's QWidgetPrivate::widgetItem and permanently disables the
 * layout size cache for that widget. That slot is private Qt state with no public
 * accessor, so it cannot be asserted from a test.
 */

namespace
{

std::atomic<long long> g_liveBlocks{0};
std::atomic<long long> g_liveBytes{0};

}

void* operator new(std::size_t size)
{
    void* pointer = std::malloc(size != 0 ? size : 1);
    if(pointer == nullptr) {
        throw std::bad_alloc();
    }
    g_liveBlocks.fetch_add(1, std::memory_order_relaxed);
    g_liveBytes.fetch_add((long long)size, std::memory_order_relaxed);
    return pointer;
}

void* operator new[](std::size_t size)
{
    return operator new(size);
}

void operator delete(void* pointer) noexcept
{
    if(pointer != nullptr) {
        g_liveBlocks.fetch_sub(1, std::memory_order_relaxed);
    }
    std::free(pointer);
}

void operator delete(void* pointer, std::size_t size) noexcept
{
    if(pointer != nullptr) {
        g_liveBytes.fetch_sub((long long)size, std::memory_order_relaxed);
    }
    operator delete(pointer);
}

void operator delete[](void* pointer) noexcept
{
    operator delete(pointer);
}

void operator delete[](void* pointer, std::size_t size) noexcept
{
    operator delete(pointer, size);
}

namespace
{

const int WarmupRounds = 20;
const int MeasuredRounds = 100;
const int ControlLeakBytes = 999983;

// The compiler is allowed to elide a new/delete pair it can see the whole life of.
// Routing the pointer through a volatile object forces the allocation to happen.
char* volatile g_controlBlock = nullptr;

long long liveBlocks()
{
    return g_liveBlocks.load(std::memory_order_relaxed);
}

/**
 * @brief Count the blocks a single QLayout::addWidget allocates.
 *
 * Every allocation in the measured window happens inside Qt: addWidget builds a
 * QWidgetItemV2 through QLayoutPrivate::createWidgetItem. The widget and the layout
 * are created before the window opens, so nothing this translation unit allocates
 * is included and the result reports only whether the counter reaches across the
 * module boundary.
 *
 * @return Number of blocks the counter attributed to the call
 */
long long blocksSeenFromInsideQt()
{
    QWidget host;
    QVBoxLayout* layout = new QVBoxLayout(&host);
    QLabel* probe = new QLabel(QStringLiteral("probe"), &host);

    const long long before = liveBlocks();
    layout->addWidget(probe);
    return liveBlocks() - before;
}

const char* BlindCounterMessage =
    "the allocation counter does not see allocations made inside Qt, so the leak "
    "assertions in this file cannot distinguish a fixed build from a broken one";

void cycleButtonAlignment(ButtonLabel* label, int rounds)
{
    for(int i = 0; i < rounds; i++) {
        label->setButtonAlignment(i % 2 == 0 ? Qt::AlignRight : Qt::AlignLeft);
    }
}

void cycleFlowLayout(FlowLayout* layout, QWidget* parent, int rounds)
{
    for(int i = 0; i < rounds; i++) {
        layout->addWidget(new QLabel(QStringLiteral("a"), parent));
        layout->addWidget(new QLabel(QStringLiteral("b"), parent));
        layout->clear();
    }
}

}

class TstLayoutItemOwnership : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief The counter balances an allocation made in this translation unit.
     *
     * This is necessary and not sufficient. The replacement is trivially in effect
     * for code compiled here, so this case passes on a platform where the counter is
     * blind to the allocations actually under test.
     */
    void allocationCounter_seesADroppedAllocation()
    {
        const long long before = liveBlocks();

        g_controlBlock = new char[ControlLeakBytes];
        QCOMPARE(liveBlocks() - before, 1LL);

        delete[] g_controlBlock;
        g_controlBlock = nullptr;
        QCOMPARE(liveBlocks() - before, 0LL);
    }

    /**
     * @brief The counter sees an allocation made inside Qt.
     *
     * This is the control the two leak cases actually depend on. Both of them assert
     * that a count did not grow, and a counter that cannot see Qt's allocations
     * satisfies that for a fixed build and a broken one alike. Asserting it here
     * makes a blind counter a failure that names its own cause rather than a silent
     * pass.
     */
    void allocationCounter_seesAllocationsMadeInsideQt()
    {
        QVERIFY2(blocksSeenFromInsideQt() >= 1, BlindCounterMessage);
    }

    /**
     * @brief ButtonLabel::relayout destroys the items it takes out of the layout.
     */
    void buttonLabelRelayout_leaksNothing()
    {
        if(blocksSeenFromInsideQt() < 1) {
            QSKIP(BlindCounterMessage);
        }

        QWidget parent;
        ButtonLabel label(QStringLiteral("hello"), &parent);

        // Absorb any one-time allocation the first relayout makes.
        cycleButtonAlignment(&label, WarmupRounds);

        const long long before = liveBlocks();
        cycleButtonAlignment(&label, MeasuredRounds);
        const long long leaked = liveBlocks() - before;

        QVERIFY2(leaked == 0,
                 qPrintable(QString("%1 blocks leaked over %2 setButtonAlignment calls")
                            .arg(leaked).arg(MeasuredRounds)));
        QCOMPARE(label.layout()->count(), 2);
    }

    /**
     * @brief FlowLayout::clear destroys both the widget and the item holding it.
     */
    void flowLayoutClear_leaksNothing()
    {
        if(blocksSeenFromInsideQt() < 1) {
            QSKIP(BlindCounterMessage);
        }

        QWidget parent;
        FlowLayout* layout = new FlowLayout(&parent);

        cycleFlowLayout(layout, &parent, WarmupRounds);

        const long long before = liveBlocks();
        cycleFlowLayout(layout, &parent, MeasuredRounds);
        const long long leaked = liveBlocks() - before;

        QVERIFY2(leaked == 0,
                 qPrintable(QString("%1 blocks leaked over %2 add/clear rounds")
                            .arg(leaked).arg(MeasuredRounds)));
        QCOMPARE(layout->count(), 0);
    }

    /**
     * @brief Both classes keep the widgets they are given.
     *
     * ButtonLabel re-adds the same label and button after every relayout, and
     * FlowLayout owns the widgets it is handed. Destroying the wrong one would show
     * up here rather than as a leak.
     */
    void relayout_keepsTheOriginalChildWidgets()
    {
        QWidget parent;
        ButtonLabel label(QStringLiteral("hello"), &parent);

        QWidget* first = label.layout()->itemAt(0)->widget();
        QWidget* second = label.layout()->itemAt(1)->widget();
        QVERIFY(first != nullptr);
        QVERIFY(second != nullptr);

        label.setButtonAlignment(Qt::AlignLeft);

        QCOMPARE(label.layout()->count(), 2);
        QWidget* afterFirst = label.layout()->itemAt(0)->widget();
        QWidget* afterSecond = label.layout()->itemAt(1)->widget();
        QVERIFY(afterFirst == first || afterFirst == second);
        QVERIFY(afterSecond == first || afterSecond == second);
        QVERIFY(afterFirst != afterSecond);
    }
};

QTEST_MAIN(TstLayoutItemOwnership)
#include "tst_layoutitemownership.moc"
