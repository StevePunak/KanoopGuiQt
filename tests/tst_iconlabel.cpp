#include <QTest>
#include <QIcon>
#include <QLayout>
#include <QLayoutItem>
#include <QPixmap>
#include <QStringList>

#include <Kanoop/gui/widgets/iconlabel.h>

namespace
{

QStringList g_warnings;
QtMessageHandler g_previousHandler = nullptr;

void captureWarnings(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    if(type == QtWarningMsg) {
        g_warnings.append(message);
    }
    if(g_previousHandler != nullptr) {
        g_previousHandler(type, context, message);
    }
}

/**
 * @brief Installs a warning-capturing message handler for its lifetime.
 *
 * qInstallMessageHandler is process-global; leaving one installed steals the
 * messages of every later test in this binary.
 */
class WarningCapture
{
public:
    WarningCapture() { g_warnings.clear(); g_previousHandler = qInstallMessageHandler(captureWarnings); }
    ~WarningCapture() { qInstallMessageHandler(g_previousHandler); g_previousHandler = nullptr; }

    int count() const { return g_warnings.count(); }
    QString joined() const { return g_warnings.join(" | "); }
};

QIcon makeIcon(const QColor& color)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(color);
    return QIcon(pixmap);
}

// The layout is built as [icon, text].
const int IconItemIndex = 0;
const int ExpectedItemCount = 2;

}

class TstIconLabel : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief The two child labels stay in the layout across repeated setText calls.
     *
     * Asserting text() alone is not enough: the getter kept returning the value that
     * had just been set while the widget painted nothing, because the labels were
     * reparented out of any layout.
     */
    void setText_keepsBothChildrenInTheLayout()
    {
        WarningCapture capture;

        IconLabel label("first");
        QVERIFY(label.layout() != nullptr);
        QCOMPARE(label.layout()->count(), ExpectedItemCount);

        label.setText("second");
        QCOMPARE(label.text(), QStringLiteral("second"));
        QCOMPARE(label.layout()->count(), ExpectedItemCount);

        label.setText("third");
        QCOMPARE(label.text(), QStringLiteral("third"));
        QCOMPARE(label.layout()->count(), ExpectedItemCount);

        // Negative property; warningCapture_seesAWarning is the control.
        QVERIFY2(capture.count() == 0, qPrintable(capture.joined()));
    }

    /**
     * @brief setIcon leaves the layout intact as well.
     */
    void setIcon_keepsBothChildrenInTheLayout()
    {
        WarningCapture capture;

        IconLabel label("text", makeIcon(Qt::red));
        QCOMPARE(label.layout()->count(), ExpectedItemCount);

        label.setIcon(makeIcon(Qt::blue));
        QCOMPARE(label.layout()->count(), ExpectedItemCount);

        label.setText("changed");
        label.setIcon(makeIcon(Qt::green));
        QCOMPARE(label.layout()->count(), ExpectedItemCount);
        QCOMPARE(label.text(), QStringLiteral("changed"));

        QVERIFY2(capture.count() == 0, qPrintable(capture.joined()));
    }

    /**
     * @brief Control for the two "no warnings" assertions above.
     *
     * Each of those is a negative property, satisfied by a handler that was never
     * installed.
     */
    void warningCapture_seesAWarning()
    {
        WarningCapture capture;
        qWarning("deliberate warning from the icon label test");
        QCOMPARE(capture.count(), 1);
    }

    /**
     * @brief setIconAlignment reaches the layout item that holds the icon.
     *
     * This member was declared and never defined, so calling it from anywhere
     * outside the library failed to link. The call itself is most of the test.
     */
    void setIconAlignment_reachesTheIconItem()
    {
        IconLabel label("text", makeIcon(Qt::red));
        QLayoutItem* iconItem = label.layout()->itemAt(IconItemIndex);
        QVERIFY(iconItem != nullptr);

        label.setIconAlignment(Qt::AlignRight);
        QCOMPARE(label.layout()->itemAt(IconItemIndex)->alignment(), Qt::Alignment(Qt::AlignRight));

        label.setIconAlignment(Qt::AlignLeft);
        QCOMPARE(label.layout()->itemAt(IconItemIndex)->alignment(), Qt::Alignment(Qt::AlignLeft));
    }

    /**
     * @brief A later setText does not discard the configured icon alignment.
     */
    void setIconAlignment_survivesALaterSetText()
    {
        IconLabel label("text", makeIcon(Qt::red));
        label.setIconAlignment(Qt::AlignRight);

        label.setText("changed");

        QCOMPARE(label.layout()->count(), ExpectedItemCount);
        QCOMPARE(label.layout()->itemAt(IconItemIndex)->alignment(), Qt::Alignment(Qt::AlignRight));
    }
};

QTEST_MAIN(TstIconLabel)
#include "tst_iconlabel.moc"
