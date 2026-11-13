#include <QMdiArea>
#include <QSettings>
#include <QTemporaryDir>
#include <QTest>
#include <Kanoop/gui/guisettings.h>
#include <Kanoop/gui/mdisubwindow.h>
#include <Kanoop/gui/mdiwindow.h>

/** @brief A sub-window whose object name and geometry kind are set by the test. */
class ChildWindow : public MainWindowBase
{
public:
    ChildWindow(const QString& name, const QString& kind) :
        MainWindowBase("child"),
        _kind(kind)
    {
        setObjectName(name);
        setDefaultSize(1200, 1200);
    }

    QString geometryKindName() const override { return _kind; }

private:
    QString _kind;
};

/** @brief An MDI host with openSubWindow() made callable. */
class HostWindow : public MdiWindow
{
public:
    HostWindow() :
        MdiWindow("host")
    {
        setCentralWidget(new QMdiArea(this));
        resize(1600, 1200);
    }

    using MdiWindow::openSubWindow;
};

/**
 * @brief What MdiWindow::openSubWindow() records for a window it placed without a stored geometry of its own.
 *
 * A window whose name differs from its kind name records the position and size it was opened at,
 * before the operator first moves or resizes it.
 */
class TstMdiSubWindowPlacement : public QObject
{
    Q_OBJECT

private:
    QTemporaryDir _tempDir;
    GuiSettings* _settings = nullptr;

    static QString subWindowName(const QString& name) { return name + MdiSubWindow::MdiSubWindowSuffix; }

    QPoint storedPosition(const QString& name) const
    {
        QPoint result(-1, -1);
        _settings->tryGetLastWindowPosition(subWindowName(name), result);
        return result;
    }

    QSize storedSize(const QString& name) const
    {
        QSize result;
        _settings->tryGetLastWindowSize(subWindowName(name), result);
        return result;
    }

private slots:
    void initTestCase()
    {
        QCoreApplication::setOrganizationName("KanoopTest");
        QCoreApplication::setApplicationName("tst_mdisubwindow_placement");
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, _tempDir.path());

        _settings = new GuiSettings();
        GuiSettings::setGlobalInstance(_settings);
    }

    void cleanupTestCase()
    {
        GuiSettings::setGlobalInstance(nullptr);
        delete _settings;
        _settings = nullptr;
    }

    void init()
    {
        QSettings settings;
        settings.clear();
        settings.sync();
    }

    void aWindowSizedFromItsKindRecordsThatSize()
    {
        _settings->setLastWindowSize(subWindowName("Thing"), QSize(900, 700));

        HostWindow host;
        host.openSubWindow(new ChildWindow("Thing_a", "Thing"), 1);

        QCOMPARE(storedSize("Thing_a"), QSize(900, 700));
    }

    void aWindowPlacedFromItsKindRecordsThatPosition()
    {
        _settings->setLastWindowPosition(subWindowName("Thing"), QPoint(40, 30));

        HostWindow host;
        host.openSubWindow(new ChildWindow("Thing_a", "Thing"), 1);

        QCOMPARE(storedPosition("Thing_a"), QPoint(40, 30));
    }

    void aCascadedWindowRecordsTheCascadedPosition()
    {
        _settings->setLastWindowPosition(subWindowName("Thing_a"), QPoint(100, 80));

        HostWindow host;
        host.openSubWindow(new ChildWindow("Thing_a", "Thing"), 1);
        host.openSubWindow(new ChildWindow("Thing_b", "Thing"), 1);

        QCOMPARE(storedPosition("Thing_b"), QPoint(120, 100));
    }

    void aWindowNamedAfterItsKindStillCascadesOffAnotherOfTheSameName()
    {
        // Two windows sharing one name share one record. Recording the first one's position on
        // open would make the second read it as its own and open on top of the first.
        HostWindow host;
        MdiSubWindow* first = host.openSubWindow(new ChildWindow("Plain", "Plain"), 2);
        MdiSubWindow* second = host.openSubWindow(new ChildWindow("Plain", "Plain"), 2);

        QCOMPARE(second->pos(), first->pos() + QPoint(20, 20));
    }

    void aWindowThatDoesNotPersistItsPositionRecordsNone()
    {
        _settings->setLastWindowPosition(subWindowName("Thing"), QPoint(40, 30));

        HostWindow host;
        ChildWindow* child = new ChildWindow("Thing_a", "Thing");
        child->setPersistPosition(false);
        MdiSubWindow* subWindow = host.openSubWindow(child, 1);

        QCOMPARE(_settings->widgetHasPersistentPosition(subWindow), false);

        // The control: a window of the same kind that persists its position does record one.
        MdiSubWindow* persisting = host.openSubWindow(new ChildWindow("Thing_b", "Thing"), 1);
        QCOMPARE(_settings->widgetHasPersistentPosition(persisting), true);
    }
};

QTEST_MAIN(TstMdiSubWindowPlacement)
#include "tst_mdisubwindow_placement.moc"
