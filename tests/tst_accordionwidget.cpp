#include <QTest>
#include <QLabel>
#include <QWidget>

#include <Kanoop/gui/widgets/accordionwidget.h>

namespace
{

/**
 * @brief Build an accordion whose child order and layout order disagree.
 *
 * Two appends followed by an insert at the front. insertItem parents the new
 * AccordionItem to the widget before inserting it, so QLayout::addChildWidget
 * performs no reparent and the child list keeps construction order while the
 * layout honours the requested position.
 */
void buildDivergentAccordion(AccordionWidget* accordion, QLabel** a, QLabel** b, QLabel** c)
{
    *a = new QLabel(QStringLiteral("content A"));
    *b = new QLabel(QStringLiteral("content B"));
    *c = new QLabel(QStringLiteral("content C"));
    accordion->addItem(QStringLiteral("A"), *a);
    accordion->addItem(QStringLiteral("B"), *b);
    accordion->insertItem(0, QStringLiteral("C"), *c);
    accordion->expandAll();
    accordion->show();
    QCoreApplication::processEvents();
}

}

class TstAccordionWidget : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief An inserted panel takes the layout position it asked for.
     */
    void insertItem_takesTheRequestedLayoutPosition()
    {
        AccordionWidget accordion;
        QLabel* a = nullptr;
        QLabel* b = nullptr;
        QLabel* c = nullptr;
        buildDivergentAccordion(&accordion, &a, &b, &c);

        QCOMPARE(accordion.count(), 3);
        QCOMPARE(accordion.indexOf(c), 0);
        QCOMPARE(accordion.indexOf(a), 1);
        QCOMPARE(accordion.indexOf(b), 2);
    }

    /**
     * @brief setItemVisible hides the panel that indexOf named.
     *
     * The observable is the caller's own content widget, which is independent of
     * both the child order and the layout order.
     */
    void setItemVisible_hidesThePanelIndexOfNames()
    {
        AccordionWidget accordion;
        QLabel* a = nullptr;
        QLabel* b = nullptr;
        QLabel* c = nullptr;
        buildDivergentAccordion(&accordion, &a, &b, &c);

        // Without this, "C is hidden" is also satisfied by nothing being visible.
        QVERIFY(a->isVisible());
        QVERIFY(b->isVisible());
        QVERIFY(c->isVisible());

        accordion.setItemVisible(accordion.indexOf(c), false);
        QCoreApplication::processEvents();

        QVERIFY2(c->isVisible() == false, "the panel indexOf named stayed visible");
        QVERIFY2(a->isVisible(), "a different panel was hidden");
        QVERIFY2(b->isVisible(), "a different panel was hidden");
    }

    /**
     * @brief An append-only accordion behaves the same way.
     *
     * Both orders agree here, so this case passed before the change too. It pins
     * that the fix relocated the index domain rather than moving the defect.
     */
    void setItemVisible_appendOnlyAccordionIsUnchanged()
    {
        AccordionWidget accordion;
        QLabel* a = new QLabel(QStringLiteral("content A"));
        QLabel* b = new QLabel(QStringLiteral("content B"));
        accordion.addItem(QStringLiteral("A"), a);
        accordion.addItem(QStringLiteral("B"), b);
        accordion.expandAll();
        accordion.show();
        QCoreApplication::processEvents();

        QVERIFY(a->isVisible());
        QVERIFY(b->isVisible());

        accordion.setItemVisible(accordion.indexOf(b), false);
        QCoreApplication::processEvents();

        QVERIFY(b->isVisible() == false);
        QVERIFY(a->isVisible());
    }

    /**
     * @brief An index at or past the end is ignored.
     */
    void setItemVisible_indexPastTheEndIsANoOp()
    {
        AccordionWidget accordion;
        QLabel* a = new QLabel(QStringLiteral("content A"));
        accordion.addItem(QStringLiteral("A"), a);
        accordion.expandAll();
        accordion.show();
        QCoreApplication::processEvents();

        accordion.setItemVisible(accordion.count(), false);
        accordion.setItemVisible(accordion.count() + 5, false);
        QCoreApplication::processEvents();

        QVERIFY(a->isVisible());
    }

    /**
     * @brief A negative index is ignored.
     *
     * The guard had an upper bound only, so this reached QList::operator[] at -1.
     * QList's bounds check is a Q_ASSERT and is compiled out in every non-Debug
     * config, so there was nothing to catch it: against 46c7f8f^ this crashes the
     * test binary outright. It is declared last so the cases above report first.
     */
    void setItemVisible_negativeIndexIsANoOp()
    {
        AccordionWidget accordion;
        QLabel* a = new QLabel(QStringLiteral("content A"));
        accordion.addItem(QStringLiteral("A"), a);
        accordion.expandAll();
        accordion.show();
        QCoreApplication::processEvents();

        accordion.setItemVisible(-1, false);
        QCoreApplication::processEvents();

        QVERIFY(a->isVisible());
    }
};

QTEST_MAIN(TstAccordionWidget)
#include "tst_accordionwidget.moc"
