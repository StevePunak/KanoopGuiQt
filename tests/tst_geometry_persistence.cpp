#include <QSettings>
#include <QTemporaryDir>
#include <QTest>
#include <QWidget>
#include <Kanoop/gui/guisettings.h>

/**
 * @brief Asking GuiSettings about a widget's stored geometry, and what the asking costs.
 *
 * ⚠ getLastWindowSize() MINTS the size key when it is absent, and widgetHasPersistentGeometry()
 * tests that same key. So the size question is answered "yes" by the act of having asked for the
 * size, and it cannot tell a genuinely restored window from one whose record was just created.
 * That is why a placement decision is keyed on the POSITION instead: reading a position computes
 * a default rather than recording one, so widgetHasPersistentPosition() stays answerable.
 *
 * The asymmetry between the two is invisible at every call site, and the two queries read as
 * interchangeable from their names. These tests pin which one records and which one does not.
 *
 * ⚠ Which query MdiWindow::openSubWindow() picks, and where a sub-window is therefore placed,
 * is not covered here.
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
        QWidget widget;
        QWidget* target = named(widget, "minted");
        QVERIFY(_settings->widgetHasPersistentGeometry(target) == false);

        _settings->getLastWindowSize(target, QSize(640, 480));

        QVERIFY2(_settings->widgetHasPersistentGeometry(target),
                 "fetching the size no longer records it; this query could now answer a "
                 "placement decision, which it currently cannot");
    }

    void askingForTheLastPositionCreatesNothing()
    {
        // ⚠ The two getters are not alike, and their names do not say so. A caller generalising
        // from this one to the other gets it exactly backwards.
        QWidget widget;
        QWidget* target = named(widget, "position-only");

        _settings->getLastWindowPosition(target, QSize(640, 480));

        QVERIFY(_settings->widgetHasPersistentGeometry(target) == false);
    }

    void aSizeThatWasActuallyStoredReadsAsStoredToo()
    {
        // Positive control for the minted-key test above.
        QWidget widget;
        QWidget* target = named(widget, "genuinely-stored");

        _settings->setLastWindowSize(target, QSize(300, 200));

        QVERIFY(_settings->widgetHasPersistentGeometry(target));
        QCOMPARE(_settings->getLastWindowSize(target, QSize(640, 480)), QSize(300, 200));
    }

    void aWidgetNobodyHasPlacedHasNoStoredPosition()
    {
        QWidget widget;
        QVERIFY(_settings->widgetHasPersistentPosition(named(widget, "unplaced")) == false);
    }

    void askingForEitherGeometryValueCreatesNoStoredPosition()
    {
        // ⚠⚠ The property a placement decision is keyed on, and the reason it is keyed on the
        // position rather than the size. Reading the size records the size -- so a size key
        // exists from the first window of a type onward -- but neither read may invent a
        // POSITION, or the cascade branch becomes unreachable for every window after the first.
        QWidget widget;
        QWidget* target = named(widget, "read-both");

        _settings->getLastWindowPosition(target, QSize(640, 480));
        _settings->getLastWindowSize(target, QSize(640, 480));

        QVERIFY2(_settings->widgetHasPersistentGeometry(target),
                 "fetching the size no longer records it");
        QVERIFY2(_settings->widgetHasPersistentPosition(target) == false,
                 "reading a geometry value invented a stored position; any placement decision "
                 "keyed on it is now unreachable after the first window of a type");
    }

    void aPositionThatWasActuallyStoredReadsAsStored()
    {
        // Positive control for the test above.
        QWidget widget;
        QWidget* target = named(widget, "placed");

        _settings->setLastWindowPosition(target, QPoint(300, 200));

        QVERIFY(_settings->widgetHasPersistentPosition(target));
        QCOMPARE(_settings->getLastWindowPosition(target, QSize(640, 480)), QPoint(300, 200));
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
