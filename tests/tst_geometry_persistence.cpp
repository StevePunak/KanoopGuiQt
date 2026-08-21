#include <QSettings>
#include <QTemporaryDir>
#include <QTest>
#include <QWidget>
#include <Kanoop/gui/guisettings.h>

/**
 * @brief Asking GuiSettings about a widget's stored geometry, and what the asking costs.
 *
 * ⚠ getLastWindowSize() MINTS the size key when it is absent, and widgetHasPersistentGeometry()
 * tests that same key. So the question "did this widget have geometry stored?" can only be
 * answered BEFORE the size is fetched -- afterwards it is always yes, and the caller cannot tell
 * a genuinely restored window from one it has just created the record for.
 *
 * That asymmetry is invisible at every call site. A caller that reads the guard after fetching
 * the size compiles clean, runs clean, and is simply always wrong; the only thing protecting it
 * is the ordering of two adjacent lines. These tests pin the contract those lines depend on.
 *
 * ⚠ What this does NOT cover: where a sub-window is actually placed. MdiWindow::openSubWindow()
 * consumes this contract and its own guard order is not reachable from here -- that needs an MDI
 * area, a main window and an ini seeded with a position the cascade could never produce.
 */
class TstGeometryPersistence : public QObject
{
    Q_OBJECT

private:
    QTemporaryDir _tempDir;
    GuiSettings* _settings = nullptr;

    /**
     * @brief A widget that is nothing but a settings key.
     *
     * ⚠ The whole geometry API keys off objectName() alone, so an unnamed widget shares one key
     * with every other unnamed widget and the tests below would read each other's records.
     */
    static QWidget* named(QWidget& widget, const QString& name)
    {
        widget.setObjectName(name);
        return &widget;
    }

private slots:
    void initTestCase()
    {
        // Redirect QSettings to a throwaway location so the test never touches the
        // real user configuration.
        QCoreApplication::setOrganizationName("KanoopTest");
        QCoreApplication::setApplicationName("tst_geometry_persistence");
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

    void aWidgetNobodyHasStoredAnythingForHasNoGeometry()
    {
        QWidget widget;
        QVERIFY(_settings->widgetHasPersistentGeometry(named(widget, "fresh")) == false);
    }

    void askingForTheLastSizeCreatesTheRecordThatAnswersTheQuestion()
    {
        // ⚠⚠ The trap, stated as an assertion. The question is answered "yes" by the act of
        // having asked something else -- so a caller that needs the answer must take it first.
        QWidget widget;
        QWidget* target = named(widget, "minted");
        QVERIFY(_settings->widgetHasPersistentGeometry(target) == false);

        _settings->getLastWindowSize(target, QSize(640, 480));

        QVERIFY2(_settings->widgetHasPersistentGeometry(target),
                 "fetching the size no longer records it; the ordering constraint in "
                 "MdiWindow::openSubWindow is now unnecessary");
    }

    void askingForTheLastPositionCreatesNothing()
    {
        // ⚠ The two getters are not alike, which is the entire reason the order matters for one
        // of them. A caller generalising from this one to the other gets it wrong.
        QWidget widget;
        QWidget* target = named(widget, "position-only");

        _settings->getLastWindowPosition(target, QSize(640, 480));

        QVERIFY(_settings->widgetHasPersistentGeometry(target) == false);
    }

    void aSizeThatWasActuallyStoredReadsAsStoredToo()
    {
        // Without this the "yes" above could only ever be the minted artefact, and the query
        // would be pinned as useless rather than as order-dependent.
        QWidget widget;
        QWidget* target = named(widget, "genuinely-stored");

        _settings->setLastWindowSize(target, QSize(300, 200));

        QVERIFY(_settings->widgetHasPersistentGeometry(target));
        QCOMPARE(_settings->getLastWindowSize(target, QSize(640, 480)), QSize(300, 200));
    }

    void theRecordTheQuestionCreatesHoldsTheDefaultItWasGiven()
    {
        // The minted record is the caller's own default, so a window that was never stored is
        // sized deliberately rather than from whatever the store happened to contain.
        QWidget widget;
        QWidget* target = named(widget, "minted-value");

        QCOMPARE(_settings->getLastWindowSize(target, QSize(640, 480)), QSize(640, 480));
        QCOMPARE(_settings->getLastWindowSize(target, QSize(1024, 768)), QSize(640, 480));
    }
};

QTEST_MAIN(TstGeometryPersistence)
#include "tst_geometry_persistence.moc"
