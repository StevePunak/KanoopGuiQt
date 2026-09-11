#include <QTest>
#include <QPersistentModelIndex>
#include <QSortFilterProxyModel>
#include <QUuid>

#include <Kanoop/entitymetadata.h>
#include <Kanoop/gui/abstractitemmodel.h>
#include <Kanoop/gui/abstractmodelitem.h>

namespace
{

QStringList g_messages;
QtMessageHandler g_previousHandler = nullptr;

void captureHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    g_messages.append(message);
    if(g_previousHandler != nullptr) {
        g_previousHandler(type, context, message);
    }
}

class Item : public AbstractModelItem
{
public:
    Item(const QString& name, const QUuid& uuid, AbstractItemModel* model) :
        AbstractModelItem(EntityMetadata(1, name, uuid), model), _name(name) {}

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
    Model() : AbstractItemModel("tst-rootitems") {}

    using AbstractItemModel::appendRootItem;
    using AbstractItemModel::appendRootItems;
    using AbstractItemModel::deleteRootItems;
    using AbstractItemModel::insertRootItem;
    using AbstractItemModel::rootItemCount;
};

/**
 * @brief Counts and records every row-removal announcement a model makes.
 */
class RemovalRecorder
{
public:
    explicit RemovalRecorder(QAbstractItemModel* model)
    {
        QObject::connect(model, &QAbstractItemModel::rowsAboutToBeRemoved, model,
                         [this](const QModelIndex& parent, int first, int last) {
            ++aboutToCount;
            sequence.append(QString("about(parentValid=%1,%2..%3)").arg(parent.isValid()).arg(first).arg(last));
        });
        QObject::connect(model, &QAbstractItemModel::rowsRemoved, model,
                         [this](const QModelIndex& parent, int first, int last) {
            ++removedCount;
            sequence.append(QString("removed(parentValid=%1,%2..%3)").arg(parent.isValid()).arg(first).arg(last));
        });
    }

    int aboutToCount = 0;
    int removedCount = 0;
    QStringList sequence;
};

/**
 * @brief Records the parent and range of the first row-insertion announcement.
 */
class InsertionRecorder
{
public:
    explicit InsertionRecorder(QAbstractItemModel* model)
    {
        QObject::connect(model, &QAbstractItemModel::rowsAboutToBeInserted, model,
                         [this](const QModelIndex& parent, int f, int l) {
            ++count;
            parentWasValid = parent.isValid();
            first = f;
            last = l;
        });
    }

    int count = 0;
    bool parentWasValid = true;
    int first = -1;
    int last = -1;
};

}

/**
 * @brief Pins AbstractItemModel's root-item insertion and removal announcements.
 *
 * ⚠ Two cases here pass against the unfixed code by design and are the positive controls for the
 * cases beside them: insertAtTheTail_announcesTheRootParent and the two append cases.  Removing one
 * as redundant leaves its neighbour's assertion satisfiable by a model that announced nothing.
 *
 * ⚠ rootAndItsDescendantSharingAUuid_removesOnlyTheRoot covers a use-after-free.  Its assertions
 * hold on unsanitized builds either way; the defect it pins is only visible when this binary is
 * built with -fsanitize=address.
 */
class TstAbstractItemModelRootItems : public QObject
{
    Q_OBJECT

private slots:
    void init()
    {
        g_messages.clear();
        g_previousHandler = qInstallMessageHandler(captureHandler);
    }

    void cleanup()
    {
        qInstallMessageHandler(g_previousHandler);
        g_previousHandler = nullptr;
    }

    void twoRootsSharingAUuid_announceOneRemovalEach()
    {
        Model model;
        const QUuid shared = QUuid::createUuid();
        Item* first = new Item("r0", QUuid::createUuid(), &model);
        Item* second = new Item("r1", shared, &model);
        Item* third = new Item("r2", shared, &model);
        Item* fourth = new Item("r3", QUuid::createUuid(), &model);
        model.appendRootItem(first);
        model.appendRootItem(second);
        model.appendRootItem(third);
        model.appendRootItem(fourth);

        QCOMPARE(model.indexesOfEntityUuid(shared).count(), 2);

        QPersistentModelIndex below(model.index(3, 0, QModelIndex()));
        QVERIFY(below.isValid());
        QCOMPARE(below.internalPointer(), fourth);

        RemovalRecorder recorder(&model);
        model.deleteRootItems(shared);

        // Control: the removal really happened, so the counts below are not satisfied by inaction.
        QCOMPARE(model.rootItemCount(), 2);
        QCOMPARE(model.index(0, 0, QModelIndex()).internalPointer(), first);

        // A double announcement leaves the same final row count, so the count is the assertion.
        QCOMPARE(recorder.aboutToCount, 2);
        QCOMPARE(recorder.removedCount, 2);

        QVERIFY(below.isValid());
        QCOMPARE(below.internalPointer(), fourth);
        QCOMPARE(below.row(), 1);
    }

    void rootAndItsDescendantSharingAUuid_removesOnlyTheRoot()
    {
        Model model;
        const QUuid shared = QUuid::createUuid();
        Item* first = new Item("r0", QUuid::createUuid(), &model);
        Item* second = new Item("r1", shared, &model);
        second->appendChild(new Item("r1-child", shared, &model));
        Item* third = new Item("r2", QUuid::createUuid(), &model);
        model.appendRootItem(first);
        model.appendRootItem(second);
        model.appendRootItem(third);

        // Control: the uuid matches the root AND its child.  A fixture that matched neither would
        // satisfy every assertion below without removing anything.
        QCOMPARE(model.indexesOfEntityUuid(shared).count(), 2);

        RemovalRecorder recorder(&model);
        model.deleteRootItems(shared);

        QCOMPARE(model.rootItemCount(), 2);
        QCOMPARE(model.index(1, 0, QModelIndex()).internalPointer(), third);
        QCOMPARE(recorder.aboutToCount, 1);
        QCOMPARE(recorder.removedCount, 1);
    }

    void removalUnderAProxy_reportsNoInconsistency()
    {
        Model model;
        QSortFilterProxyModel proxy;
        proxy.setSourceModel(&model);

        const QUuid shared = QUuid::createUuid();
        model.appendRootItem(new Item("r0", QUuid::createUuid(), &model));
        model.appendRootItem(new Item("r1", shared, &model));
        model.appendRootItem(new Item("r2", shared, &model));
        model.appendRootItem(new Item("r3", QUuid::createUuid(), &model));

        QCOMPARE(proxy.rowCount(QModelIndex()), 4);

        g_messages.clear();
        model.deleteRootItems(shared);

        // Controls: the proxy really saw the removal, so "no complaint" is not satisfied by a proxy
        // that was never told anything.
        QCOMPARE(proxy.rowCount(QModelIndex()), 2);
        QCOMPARE(proxy.data(proxy.index(0, 0, QModelIndex()), Qt::DisplayRole).toString(), QString("r0"));

        QCOMPARE(g_messages.filter("QSortFilterProxyModel").count(), 0);
    }

    void insertAtAMiddleRow_announcesTheRootParent()
    {
        Model model;
        Item* first = new Item("r0", QUuid::createUuid(), &model);
        Item* second = new Item("r1", QUuid::createUuid(), &model);
        model.appendRootItem(first);
        model.appendRootItem(second);
        model.appendRootItem(new Item("r2", QUuid::createUuid(), &model));

        QPersistentModelIndex atOne(model.index(1, 0, QModelIndex()));
        QVERIFY(atOne.isValid());
        QCOMPARE(atOne.internalPointer(), second);

        InsertionRecorder recorder(&model);
        Item* inserted = new Item("inserted", QUuid::createUuid(), &model);
        model.insertRootItem(1, inserted);

        QCOMPARE(model.rootItemCount(), 4);
        QCOMPARE(model.index(1, 0, QModelIndex()).internalPointer(), inserted);

        QCOMPARE(recorder.count, 1);
        QCOMPARE(recorder.parentWasValid, false);
        QCOMPARE(recorder.first, 1);

        QVERIFY(atOne.isValid());
        QCOMPARE(atOne.internalPointer(), second);
        QCOMPARE(atOne.row(), 2);
    }

    void insertAtTheTail_announcesTheRootParent()
    {
        Model model;
        model.appendRootItem(new Item("r0", QUuid::createUuid(), &model));
        model.appendRootItem(new Item("r1", QUuid::createUuid(), &model));

        InsertionRecorder recorder(&model);
        Item* inserted = new Item("tail", QUuid::createUuid(), &model);
        model.insertRootItem(model.rootItemCount(), inserted);

        QCOMPARE(model.rootItemCount(), 3);
        QCOMPARE(model.index(2, 0, QModelIndex()).internalPointer(), inserted);
        QCOMPARE(recorder.count, 1);
        QCOMPARE(recorder.parentWasValid, false);
        QCOMPARE(recorder.first, 2);
    }

    void appendAnnouncesTheRootParent()
    {
        Model model;
        Item* first = new Item("r0", QUuid::createUuid(), &model);
        Item* second = new Item("r1", QUuid::createUuid(), &model);
        model.appendRootItem(first);
        model.appendRootItem(second);

        QPersistentModelIndex atZero(model.index(0, 0, QModelIndex()));
        QPersistentModelIndex atOne(model.index(1, 0, QModelIndex()));

        InsertionRecorder recorder(&model);
        Item* appended = new Item("appended", QUuid::createUuid(), &model);
        model.appendRootItem(appended);

        QCOMPARE(model.rootItemCount(), 3);
        QCOMPARE(model.index(2, 0, QModelIndex()).internalPointer(), appended);
        QCOMPARE(recorder.count, 1);
        QCOMPARE(recorder.parentWasValid, false);
        QCOMPARE(recorder.first, 2);
        QCOMPARE(recorder.last, 2);

        QVERIFY(atZero.isValid());
        QCOMPARE(atZero.internalPointer(), first);
        QCOMPARE(atZero.row(), 0);
        QVERIFY(atOne.isValid());
        QCOMPARE(atOne.internalPointer(), second);
        QCOMPARE(atOne.row(), 1);
    }

    void appendBatchAnnouncesOneInsertionAtTheRootParent()
    {
        Model model;
        Item* first = new Item("r0", QUuid::createUuid(), &model);
        model.appendRootItem(first);
        model.appendRootItem(new Item("r1", QUuid::createUuid(), &model));

        QPersistentModelIndex atZero(model.index(0, 0, QModelIndex()));

        InsertionRecorder recorder(&model);
        QList<AbstractModelItem*> batch;
        batch.append(new Item("b0", QUuid::createUuid(), &model));
        batch.append(new Item("b1", QUuid::createUuid(), &model));
        batch.append(new Item("b2", QUuid::createUuid(), &model));
        model.appendRootItems(batch);

        QCOMPARE(model.rootItemCount(), 5);
        QCOMPARE(model.index(2, 0, QModelIndex()).internalPointer(), batch.at(0));
        QCOMPARE(model.index(4, 0, QModelIndex()).internalPointer(), batch.at(2));
        QCOMPARE(recorder.count, 1);
        QCOMPARE(recorder.parentWasValid, false);
        QCOMPARE(recorder.first, 2);
        QCOMPARE(recorder.last, 4);

        QVERIFY(atZero.isValid());
        QCOMPARE(atZero.internalPointer(), first);
        QCOMPARE(atZero.row(), 0);
    }

    void emptyBatchAnnouncesNothing()
    {
        Model model;
        model.appendRootItem(new Item("r0", QUuid::createUuid(), &model));

        InsertionRecorder recorder(&model);
        model.appendRootItems(QList<AbstractModelItem*>());

        QCOMPARE(recorder.count, 0);
        QCOMPARE(model.rootItemCount(), 1);

        // Control: the same recorder does fire for a batch that has something in it.
        QList<AbstractModelItem*> batch;
        batch.append(new Item("b0", QUuid::createUuid(), &model));
        model.appendRootItems(batch);
        QCOMPARE(recorder.count, 1);
        QCOMPARE(model.rootItemCount(), 2);
    }
};

QTEST_MAIN(TstAbstractItemModelRootItems)
#include "tst_abstractitemmodel_rootitems.moc"
