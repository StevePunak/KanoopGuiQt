#include <QTest>
#include <QEvent>
#include <QResizeEvent>
#include <QStringList>
#include <QWidget>

#include <Kanoop/gui/widgets/toastmanager.h>

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

/**
 * @brief Return the toast widget parented to the given widget.
 *
 * ToastWidget is a private class with no exported symbol, so it is identified by
 * its metaobject class name rather than a cast.
 */
QWidget* findToast(QWidget* parent)
{
    for(QWidget* widget : parent->findChildren<QWidget*>(Qt::FindDirectChildrenOnly)) {
        if(QString(widget->metaObject()->className()) == QStringLiteral("ToastWidget")) {
            return widget;
        }
    }
    return nullptr;
}

const int ParentWidth = 800;
const int ParentHeight = 600;
const int ExplicitWidth = 300;
const char* ToastText = "A short toast message.";

}

class TstToastManager : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief A freshly constructed manager reports a usable configured width.
     */
    void defaultSize_isUsable()
    {
        QWidget parent;
        ToastManager manager(&parent);
        QVERIFY2(manager.size().width() > 0,
                 qPrintable(QString("configured size reported as %1x%2")
                            .arg(manager.size().width()).arg(manager.size().height())));
    }

    /**
     * @brief A manager whose parent has not resized yet still produces a usable toast.
     */
    void defaultWidth_producesAUsableToast()
    {
        WarningCapture capture;

        QWidget parent;
        parent.resize(ParentWidth, ParentHeight);
        parent.show();

        ToastManager manager(&parent);
        manager.message(ToastText);

        QWidget* toast = findToast(&parent);
        QVERIFY(toast != nullptr);
        QVERIFY2(toast->width() > 0, qPrintable(QString("toast width %1").arg(toast->width())));
        QVERIFY2(toast->height() > 0 && toast->height() < 200,
                 qPrintable(QString("one short line wrapped to height %1").arg(toast->height())));

        // Negative property. The control below proves the handler is live.
        QVERIFY2(capture.count() == 0, qPrintable(capture.joined()));
    }

    /**
     * @brief An explicitly configured width is honoured exactly.
     *
     * Control for defaultWidth_producesAUsableToast: without it, "width > 0" is
     * also satisfied by a build that hands every toast a width of 1.
     */
    void explicitWidth_isHonouredExactly()
    {
        WarningCapture capture;

        QWidget parent;
        parent.resize(ParentWidth, ParentHeight);
        parent.show();

        ToastManager manager(&parent);
        manager.resize(QSize(ExplicitWidth, 300));
        manager.message(ToastText);

        QWidget* toast = findToast(&parent);
        QVERIFY(toast != nullptr);
        QCOMPARE(toast->width(), ExplicitWidth);
        QVERIFY(toast->height() > 0 && toast->height() < 200);
        QVERIFY2(capture.count() == 0, qPrintable(capture.joined()));
    }

    /**
     * @brief Control for both "no warnings" assertions above.
     *
     * Each of those is a negative property, satisfied by a handler that was never
     * installed. This proves the capture mechanism sees a warning when one is emitted.
     */
    void warningCapture_seesAWarning()
    {
        WarningCapture capture;
        qWarning("deliberate warning from the toast manager test");
        QCOMPARE(capture.count(), 1);
    }

    /**
     * @brief An event on the parent that is not a resize leaves the toasts where they are.
     *
     * performLayout() recomputes the same position, so the only way to observe whether
     * it ran is to displace a toast by hand first and see whether it is put back.
     */
    void nonResizeParentEvent_doesNotRelayout()
    {
        QWidget parent;
        parent.resize(ParentWidth, ParentHeight);
        parent.show();

        ToastManager manager(&parent);
        manager.resize(QSize(ExplicitWidth, 300));
        manager.message(ToastText);

        QWidget* toast = findToast(&parent);
        QVERIFY(toast != nullptr);

        toast->move(0, 0);
        QEvent layoutRequest(QEvent::LayoutRequest);
        QCoreApplication::sendEvent(&parent, &layoutRequest);

        QCOMPARE(toast->y(), 0);
    }

    /**
     * @brief A parent resize still relayouts the toasts.
     *
     * Control for nonResizeParentEvent_doesNotRelayout: "the toast did not move" is
     * equally true of a manager whose layout code never runs at all.
     */
    void parentResize_doesRelayout()
    {
        QWidget parent;
        parent.resize(ParentWidth, ParentHeight);
        parent.show();

        ToastManager manager(&parent);
        manager.resize(QSize(ExplicitWidth, 300));
        manager.message(ToastText);

        QWidget* toast = findToast(&parent);
        QVERIFY(toast != nullptr);
        const int parked = ParentHeight - toast->height() - 2;
        QCOMPARE(toast->y(), parked);

        toast->move(0, 0);
        QResizeEvent resize(QSize(ParentWidth, ParentHeight), QSize(ParentWidth, ParentHeight));
        QCoreApplication::sendEvent(&parent, &resize);

        QCOMPARE(toast->y(), parked);
    }

    /**
     * @brief A real parent resize moves the toasts to the new bottom edge.
     */
    void parentResize_movesToastsToTheNewBottom()
    {
        QWidget parent;
        parent.resize(ParentWidth, ParentHeight);
        parent.show();

        ToastManager manager(&parent);
        manager.resize(QSize(ExplicitWidth, 300));
        manager.message(ToastText);

        QWidget* toast = findToast(&parent);
        QVERIFY(toast != nullptr);
        const int height = toast->height();
        QCOMPARE(toast->y(), ParentHeight - height - 2);

        parent.resize(ParentWidth, 400);
        QCoreApplication::processEvents();

        QCOMPARE(toast->y(), 400 - height - 2);
    }
};

QTEST_MAIN(TstToastManager)
#include "tst_toastmanager.moc"
