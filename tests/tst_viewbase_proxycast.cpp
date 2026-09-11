#include <QTest>
#include <QCoreApplication>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QUuid>

#include <Kanoop/entitymetadata.h>
#include <Kanoop/gui/abstractitemmodel.h>
#include <Kanoop/gui/abstractmodelitem.h>
#include <Kanoop/gui/listview.h>
#include <Kanoop/gui/tableviewbase.h>
#include <Kanoop/gui/treeviewbase.h>

namespace
{

class Item : public AbstractModelItem
{
public:
    Item(const QString& name, AbstractItemModel* model) :
        AbstractModelItem(EntityMetadata(1, name, QUuid::createUuid()), model), _name(name) {}

    QVariant data(const QModelIndex& index, int role) const override
    {
        if(role == Qt::DisplayRole) {
            return _name;
        }
        return AbstractModelItem::data(index, role);
    }

private:
    QString _name;
};

class Model : public AbstractItemModel
{
public:
    Model() : AbstractItemModel("tst-proxycast")
    {
        appendColumnHeader(1, QString("Name"));
        appendRootItem(new Item("row0", this));
        appendRootItem(new Item("row1", this));
    }
};

/**
 * @brief A QAbstractItemModel that is deliberately NOT an AbstractItemModel.
 */
class ForeignModel : public QStandardItemModel
{
public:
    ForeignModel()
    {
        setRowCount(2);
        setColumnCount(1);
    }
};

}

/**
 * @brief Pins the proxy source-model downcast in ListView, TableViewBase and TreeViewBase (8df55e2).
 *
 * ⚠ KNOWN GAP, and it is a property of the fix rather than of this file.  The fix pairs
 * dynamic_cast with Q_ASSERT(_sourceModel).  In a Debug configuration Qt does not define
 * QT_NO_DEBUG, so that assert is live inside the library and a foreign source model aborts the
 * process before sourceModel() can be read - measured as
 * `ASSERT: "_sourceModel" in .../listview.cpp:28`.  The foreign-source cases therefore assert the
 * returned POINTER only where the assert is compiled out, and QSKIP otherwise.  They are real
 * coverage in Release and RelWithDebInfo and nothing at all in Debug, so they must never be the
 * only cases in this file - the well-typed controls below run in every configuration.
 */
class TstViewBaseProxyCast : public QObject
{
    Q_OBJECT

private:
    /**
     * @brief A proxy over a real AbstractItemModel is handed back unchanged.
     */
    template <typename ViewT>
    void checkProxyOverRealModel()
    {
        Model model;
        QSortFilterProxyModel proxy;
        proxy.setSourceModel(&model);

        ViewT view;
        QCOMPARE(view.sourceModel(), nullptr);      // nothing set yet
        view.setModel(&proxy);

        QCOMPARE(view.sourceModel(), &model);
        QCOMPARE(view.model(), &proxy);
    }

    /**
     * @brief A model set directly, with no proxy, is handed back unchanged.
     */
    template <typename ViewT>
    void checkDirectModel()
    {
        Model model;
        ViewT view;
        view.setModel(&model);

        QCOMPARE(view.sourceModel(), &model);
        QCOMPARE(view.model(), &model);
    }

    /**
     * @brief A proxy whose source is some other QAbstractItemModel yields nullptr.
     */
    template <typename ViewT>
    void checkProxyOverForeignModel()
    {
#ifdef QT_NO_DEBUG
        ForeignModel foreign;
        QSortFilterProxyModel proxy;
        proxy.setSourceModel(&foreign);

        ViewT view;
        view.setModel(&proxy);

        QCOMPARE(view.sourceModel(), nullptr);
        QCOMPARE(view.model(), &proxy);
#else
        QSKIP("Q_ASSERT(_sourceModel) is live in a Debug build and aborts before sourceModel() "
              "can be read; this case is only observable where QT_NO_DEBUG is defined");
#endif
    }

private slots:
    void initTestCase()
    {
        // Keep any incidental QSettings write out of the shared EPC Power/Pulse.ini.
        QCoreApplication::setOrganizationName("KanoopTests");
        QCoreApplication::setApplicationName("tst_viewbase_proxycast");
    }

    void listView_proxyOverRealModel() { checkProxyOverRealModel<ListView>(); }
    void tableView_proxyOverRealModel() { checkProxyOverRealModel<TableViewBase>(); }
    void treeView_proxyOverRealModel() { checkProxyOverRealModel<TreeViewBase>(); }

    void listView_directModel() { checkDirectModel<ListView>(); }
    void tableView_directModel() { checkDirectModel<TableViewBase>(); }
    void treeView_directModel() { checkDirectModel<TreeViewBase>(); }

    void listView_proxyOverForeignModel() { checkProxyOverForeignModel<ListView>(); }
    void tableView_proxyOverForeignModel() { checkProxyOverForeignModel<TableViewBase>(); }
    void treeView_proxyOverForeignModel() { checkProxyOverForeignModel<TreeViewBase>(); }

    /**
     * @brief The foreign model really is a QAbstractItemModel and really is not an
     *        AbstractItemModel, so the cases above are aimed at the case they claim.
     */
    void foreignModelIsTheShapeUnderTest()
    {
        ForeignModel foreign;
        QVERIFY(qobject_cast<QAbstractItemModel*>(&foreign) != nullptr);
        QVERIFY(dynamic_cast<AbstractItemModel*>(static_cast<QAbstractItemModel*>(&foreign)) == nullptr);

        Model model;
        QVERIFY(dynamic_cast<AbstractItemModel*>(static_cast<QAbstractItemModel*>(&model)) == &model);
    }
};

QTEST_MAIN(TstViewBaseProxyCast)
#include "tst_viewbase_proxycast.moc"
