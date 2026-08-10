#include <QTest>
#include <Kanoop/gui/mainwindowbase.h>

/**
 * Exposes the protected geometry arithmetic. boundRectToArea() is static and reads no member
 * state, so no instance is ever constructed and no screen is required.
 */
class TestWindow : public MainWindowBase
{
public:
    using MainWindowBase::boundRectToArea;
};

class TstWindowGeometry : public QObject
{
    Q_OBJECT

private slots:
    void rectInsideWorkArea_unchanged()
    {
        QRect available(0, 0, 1920, 1040);
        QRect frame(100, 80, 800, 600);

        QCOMPARE(TestWindow::boundRectToArea(frame, available, QSize()), frame);
    }

    void oversizedRect_cappedAndPinnedToOrigin()
    {
        QRect available(0, 0, 1920, 1040);
        QRect frame(0, 0, 3840, 2160);

        QRect result = TestWindow::boundRectToArea(frame, available, QSize());

        QCOMPARE(result.size(), available.size());
        QCOMPARE(result.topLeft(), available.topLeft());
    }

    void frameTallerThanWorkArea_bottomLandsOnWorkAreaBottom()
    {
        // A 1920x1040 work area with a 31px title bar: the client height the caller persisted was
        // the full 1040, so the frame is 1071 tall and overflows. Bounding the frame - not the
        // client - is what keeps the bottom edge out from under the taskbar.
        QRect available(0, 0, 1920, 1040);
        QSize decoration(0, 31);
        QRect frame(0, 0, 1920, 1040 + decoration.height());

        QRect result = TestWindow::boundRectToArea(frame, available, QSize());

        QCOMPARE(result.height(), 1040);
        QCOMPARE(result.bottom(), available.bottom());
        // What actually reaches resize() is the client size, which is the frame less decoration.
        QCOMPARE(result.height() - decoration.height(), 1009);
    }

    void rectPastRightAndBottomEdges_slidesBackInside()
    {
        QRect available(0, 0, 1920, 1040);
        QRect frame(1800, 1000, 400, 200);

        QRect result = TestWindow::boundRectToArea(frame, available, QSize());

        QCOMPARE(result.size(), frame.size());
        QCOMPARE(result.topLeft(), QPoint(1520, 840));
        QVERIFY(available.contains(result));
    }

    void rectPastLeftAndTopEdges_pinnedToOrigin()
    {
        QRect available(0, 0, 1920, 1040);
        QRect frame(-100, -50, 800, 600);

        QRect result = TestWindow::boundRectToArea(frame, available, QSize());

        QCOMPARE(result.size(), frame.size());
        QCOMPARE(result.topLeft(), QPoint(0, 0));
    }

    void workAreaWithNonZeroOrigin_resultLandsInsideIt()
    {
        // Secondary monitor to the right of the primary.
        QRect available(1920, 0, 3840, 2120);
        QRect frame(6000, 2000, 800, 600);

        QRect result = TestWindow::boundRectToArea(frame, available, QSize());

        QCOMPARE(result.size(), frame.size());
        QVERIFY(available.contains(result));
        QCOMPARE(result.topLeft(), QPoint(4960, 1520));
    }

    void workAreaLeftOfPrimary_resultStaysOnThatMonitor()
    {
        // A monitor placed left of the primary has negative coordinates. Bounding must be against
        // that work area's own edges, never against zero, or the window is dragged to the primary.
        QRect available(-2560, 0, 1707, 1019);
        QRect frame(-1727, 148, 1650, 763);

        QRect result = TestWindow::boundRectToArea(frame, available, QSize(800, 600));

        QCOMPARE(result.size(), frame.size());
        QVERIFY(result.left() < 0);
        QVERIFY(available.contains(result));
        QCOMPARE(result.topLeft(), QPoint(-2503, 148));
    }

    void undersizedRect_raisedToFloor()
    {
        QRect available(0, 0, 1920, 1040);
        QRect frame(100, 80, 40, 30);

        QRect result = TestWindow::boundRectToArea(frame, available, QSize(800, 600));

        QCOMPARE(result.size(), QSize(800, 600));
        QCOMPARE(result.topLeft(), frame.topLeft());
    }

    void floorLargerThanWorkArea_cappedByWorkArea()
    {
        QRect available(0, 0, 800, 560);
        QRect frame(0, 0, 40, 30);

        QRect result = TestWindow::boundRectToArea(frame, available, QSize(800, 600));

        QCOMPARE(result.size(), available.size());
        QVERIFY(available.contains(result));
    }

    void floorPushesRectPastEdge_slidesBackInside()
    {
        QRect available(0, 0, 1920, 1040);
        QRect frame(1800, 900, 40, 30);

        QRect result = TestWindow::boundRectToArea(frame, available, QSize(800, 600));

        QCOMPARE(result.size(), QSize(800, 600));
        QCOMPARE(result.topLeft(), QPoint(1120, 440));
        QVERIFY(available.contains(result));
    }

    void invalidFloor_leavesSizeAlone()
    {
        QRect available(0, 0, 1920, 1040);
        QRect frame(100, 80, 40, 30);

        QRect result = TestWindow::boundRectToArea(frame, available, QSize());

        QCOMPARE(result, frame);
    }
};

QTEST_MAIN(TstWindowGeometry)

#include "tst_windowgeometry.moc"
