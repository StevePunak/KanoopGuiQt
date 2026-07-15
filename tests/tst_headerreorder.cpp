#include <QTest>
#include <QHeaderView>
#include <QSettings>
#include <QTableView>
#include <QTemporaryDir>

#include <Kanoop/gui/abstracttablemodel.h>
#include <Kanoop/gui/guisettings.h>

/**
 * @brief Integration test for GuiSettings column visual-order persistence.
 *
 * Exercises the save/restore path added for SC-5890: dragging a column reorders
 * the header's visual sections, the order is persisted through the model-keyed
 * header state, and it is replayed onto a fresh view via moveSection. Also covers
 * the safety gate that skips the reorder when the column set no longer matches.
 */
class TstHeaderReorder : public QObject
{
    Q_OBJECT

private:
    QTemporaryDir _tempDir;
    GuiSettings* _settings = nullptr;

    /** @brief AbstractTableModel is directly instantiable; expose the protected header API for the test. */
    class TestTableModel : public AbstractTableModel
    {
    public:
        explicit TestTableModel(QObject* parent = nullptr) : AbstractTableModel(parent) {}
        using AbstractTableModel::appendColumnHeader;
    };

    TestTableModel* makeModel(QObject* parent) const
    {
        TestTableModel* model = new TestTableModel(parent);
        model->appendColumnHeader(0, "Alpha");
        model->appendColumnHeader(1, "Beta");
        model->appendColumnHeader(2, "Gamma");
        model->appendColumnHeader(3, "Delta");
        return model;
    }

private slots:
    void initTestCase()
    {
        // Redirect QSettings to a throwaway location so the test never touches the
        // real user configuration.
        QCoreApplication::setOrganizationName("KanoopTest");
        QCoreApplication::setApplicationName("tst_headerreorder");
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

    void savedVisualOrder_isRestoredOntoFreshView()
    {
        // Save from a view whose Delta column has been dragged to the front.
        {
            QTableView view;
            view.setObjectName("reorderView");
            TestTableModel* model = makeModel(&view);
            view.setModel(model);

            QHeaderView* header = view.horizontalHeader();
            header->moveSection(header->visualIndex(3), 0);
            QCOMPARE(header->logicalIndex(0), 3);   // sanity: Delta now leads

            _settings->saveLastHeaderState(header, model);
        }

        // Restore onto a brand-new, identity-ordered view.
        {
            QTableView view;
            view.setObjectName("reorderView");
            TestTableModel* model = makeModel(&view);
            view.setModel(model);

            QHeaderView* header = view.horizontalHeader();
            QCOMPARE(header->logicalIndex(0), 0);   // starts identity

            _settings->restoreLastHeaderState(header, model);

            // Delta (logical 3) back at the front, the rest shifted right.
            QCOMPARE(header->logicalIndex(0), 3);
            QCOMPARE(header->logicalIndex(1), 0);
            QCOMPARE(header->logicalIndex(2), 1);
            QCOMPARE(header->logicalIndex(3), 2);
        }
    }

    void changedColumnSet_skipsReorder()
    {
        // Save a reordered 4-column layout.
        {
            QTableView view;
            view.setObjectName("mismatchView");
            TestTableModel* model = makeModel(&view);
            view.setModel(model);

            QHeaderView* header = view.horizontalHeader();
            header->moveSection(header->visualIndex(3), 0);
            _settings->saveLastHeaderState(header, model);
        }

        // Restore onto a view whose column text no longer matches: the reorder must
        // be skipped entirely rather than smearing a stale order onto new columns.
        {
            QTableView view;
            view.setObjectName("mismatchView");
            TestTableModel* model = new TestTableModel(&view);
            model->appendColumnHeader(0, "Alpha");
            model->appendColumnHeader(1, "DIFFERENT");
            model->appendColumnHeader(2, "Gamma");
            model->appendColumnHeader(3, "Delta");
            view.setModel(model);

            QHeaderView* header = view.horizontalHeader();
            _settings->restoreLastHeaderState(header, model);

            // Order left as identity.
            QCOMPARE(header->logicalIndex(0), 0);
            QCOMPARE(header->logicalIndex(1), 1);
            QCOMPARE(header->logicalIndex(2), 2);
            QCOMPARE(header->logicalIndex(3), 3);
        }
    }
};

QTEST_MAIN(TstHeaderReorder)
#include "tst_headerreorder.moc"
