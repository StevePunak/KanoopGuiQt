#include <QTest>
#include <Kanoop/gui/abstractmodelitem.h>
#include <Kanoop/entitymetadata.h>

class TstAbstractModelItem : public QObject
{
    Q_OBJECT

private slots:
    void defaultConstructor()
    {
        AbstractModelItem item;
        QVERIFY(item.uuid().isNull());
        QVERIFY(item.parent() == nullptr);
        QVERIFY(item.model() == nullptr);
        QCOMPARE(item.childCount(), 0);
    }

    void metadataConstructor_setsFields()
    {
        QUuid id = QUuid::createUuid();
        EntityMetadata meta(42);
        AbstractModelItem item(meta, nullptr, id);

        QCOMPARE(item.entityType(), 42);
        QCOMPARE(item.uuid(), id);
        QCOMPARE(item.entityMetadata().type(), 42);
    }

    void appendChild_addsChild()
    {
        AbstractModelItem parent;
        AbstractModelItem* child = new AbstractModelItem();
        parent.appendChild(child);

        QCOMPARE(parent.childCount(), 1);
        QCOMPARE(parent.child(0), child);
        QCOMPARE(child->parent(), &parent);
    }

    void appendChild_multipleChildren()
    {
        AbstractModelItem parent;
        AbstractModelItem* c1 = new AbstractModelItem();
        AbstractModelItem* c2 = new AbstractModelItem();
        AbstractModelItem* c3 = new AbstractModelItem();

        parent.appendChild(c1);
        parent.appendChild(c2);
        parent.appendChild(c3);

        QCOMPARE(parent.childCount(), 3);
        QCOMPARE(parent.child(0), c1);
        QCOMPARE(parent.child(1), c2);
        QCOMPARE(parent.child(2), c3);
    }

    void insertChild_atIndex()
    {
        AbstractModelItem parent;
        AbstractModelItem* c1 = new AbstractModelItem();
        AbstractModelItem* c2 = new AbstractModelItem();
        AbstractModelItem* c3 = new AbstractModelItem();

        parent.appendChild(c1);
        parent.appendChild(c3);
        parent.insertChild(1, c2);

        QCOMPARE(parent.childCount(), 3);
        QCOMPARE(parent.child(0), c1);
        QCOMPARE(parent.child(1), c2);
        QCOMPARE(parent.child(2), c3);
    }

    void child_outOfRange_returnsNull()
    {
        AbstractModelItem parent;
        QVERIFY(parent.child(0) == nullptr);
        QVERIFY(parent.child(99) == nullptr);
    }

    void deleteChild_removesChild()
    {
        AbstractModelItem parent;
        AbstractModelItem* c1 = new AbstractModelItem();
        AbstractModelItem* c2 = new AbstractModelItem();

        parent.appendChild(c1);
        parent.appendChild(c2);
        QCOMPARE(parent.childCount(), 2);

        parent.deleteChild(c1);
        QCOMPARE(parent.childCount(), 1);
        QCOMPARE(parent.child(0), c2);
    }

    void deleteAllChildren_clearsAll()
    {
        AbstractModelItem parent;
        parent.appendChild(new AbstractModelItem());
        parent.appendChild(new AbstractModelItem());
        parent.appendChild(new AbstractModelItem());
        QCOMPARE(parent.childCount(), 3);

        parent.deleteAllChildren();
        QCOMPARE(parent.childCount(), 0);
    }

    void row_rootItem_returnsZero()
    {
        AbstractModelItem item;
        QCOMPARE(item.row(), 0);
    }

    void row_childItem_returnsIndex()
    {
        AbstractModelItem parent;
        AbstractModelItem* c0 = new AbstractModelItem();
        AbstractModelItem* c1 = new AbstractModelItem();
        AbstractModelItem* c2 = new AbstractModelItem();

        parent.appendChild(c0);
        parent.appendChild(c1);
        parent.appendChild(c2);

        QCOMPARE(c0->row(), 0);
        QCOMPARE(c1->row(), 1);
        QCOMPARE(c2->row(), 2);
    }

    void siblings_returnsOtherChildren()
    {
        AbstractModelItem parent;
        AbstractModelItem* c0 = new AbstractModelItem();
        AbstractModelItem* c1 = new AbstractModelItem();
        AbstractModelItem* c2 = new AbstractModelItem();

        parent.appendChild(c0);
        parent.appendChild(c1);
        parent.appendChild(c2);

        AbstractModelItem::List sibs = c1->siblings();
        QCOMPARE(sibs.count(), 2);
        QVERIFY(sibs.contains(c0));
        QVERIFY(sibs.contains(c2));
        QVERIFY(!sibs.contains(c1));
    }

    void siblings_rootItem_returnsEmpty()
    {
        AbstractModelItem item;
        QCOMPARE(item.siblings().count(), 0);
    }

    void childCount_filteredByEntityType()
    {
        AbstractModelItem parent;

        EntityMetadata metaA(10);
        EntityMetadata metaB(20);

        parent.appendChild(new AbstractModelItem(metaA, nullptr));
        parent.appendChild(new AbstractModelItem(metaB, nullptr));
        parent.appendChild(new AbstractModelItem(metaA, nullptr));

        QCOMPARE(parent.childCount(), 3);
        QCOMPARE(parent.childCount(10), 2);
        QCOMPARE(parent.childCount(20), 1);
        QCOMPARE(parent.childCount(99), 0);
    }

    void childCountRecursive_countsGrandchildren()
    {
        AbstractModelItem root;
        AbstractModelItem* child = new AbstractModelItem();
        root.appendChild(child);
        child->appendChild(new AbstractModelItem());
        child->appendChild(new AbstractModelItem());

        QCOMPARE(root.childCountRecursive(), 3); // 1 child + 2 grandchildren
    }

    void childCountRecursive_filteredByEntityType()
    {
        EntityMetadata metaA(10);
        EntityMetadata metaB(20);

        AbstractModelItem root;
        AbstractModelItem* child = new AbstractModelItem(metaA, nullptr);
        root.appendChild(child);
        child->appendChild(new AbstractModelItem(metaA, nullptr));
        child->appendChild(new AbstractModelItem(metaB, nullptr));

        QCOMPARE(root.childCountRecursive(10), 2); // child + first grandchild
        QCOMPARE(root.childCountRecursive(20), 1); // second grandchild
    }

    // --- AbstractModelItem::List tests ---

    void list_findByUuid_found()
    {
        QUuid id1 = QUuid::createUuid();
        QUuid id2 = QUuid::createUuid();

        AbstractModelItem parent;
        AbstractModelItem* c1 = new AbstractModelItem(EntityMetadata(), nullptr, id1);
        AbstractModelItem* c2 = new AbstractModelItem(EntityMetadata(), nullptr, id2);
        parent.appendChild(c1);
        parent.appendChild(c2);

        QCOMPARE(parent.children().findByUuid(id2), c2);
    }

    void list_findByUuid_notFound()
    {
        AbstractModelItem parent;
        parent.appendChild(new AbstractModelItem());

        QVERIFY(parent.children().findByUuid(QUuid::createUuid()) == nullptr);
    }

    void list_indexOfUuid()
    {
        QUuid id = QUuid::createUuid();

        AbstractModelItem parent;
        parent.appendChild(new AbstractModelItem());
        parent.appendChild(new AbstractModelItem(EntityMetadata(), nullptr, id));

        QCOMPARE(parent.children().indexOfUuid(id), 1);
        QCOMPARE(parent.children().indexOfUuid(QUuid::createUuid()), -1);
    }

    void list_firstIndexOfEntityType()
    {
        EntityMetadata metaA(10);
        EntityMetadata metaB(20);

        AbstractModelItem parent;
        parent.appendChild(new AbstractModelItem(metaB, nullptr));
        parent.appendChild(new AbstractModelItem(metaA, nullptr));
        parent.appendChild(new AbstractModelItem(metaA, nullptr));

        QCOMPARE(parent.children().firstIndexOfEntityType(10), 1);
        QCOMPARE(parent.children().firstIndexOfEntityType(20), 0);
        QCOMPARE(parent.children().firstIndexOfEntityType(99), -1);
    }

    void list_lastIndexOfEntityType()
    {
        EntityMetadata metaA(10);
        EntityMetadata metaB(20);

        AbstractModelItem parent;
        parent.appendChild(new AbstractModelItem(metaA, nullptr));
        parent.appendChild(new AbstractModelItem(metaB, nullptr));
        parent.appendChild(new AbstractModelItem(metaA, nullptr));

        QCOMPARE(parent.children().lastIndexOfEntityType(10), 2);
        QCOMPARE(parent.children().lastIndexOfEntityType(20), 1);
    }

    void setEntityMetadata_works()
    {
        AbstractModelItem item;
        EntityMetadata meta(77);
        item.setEntityMetadata(meta);
        QCOMPARE(item.entityType(), 77);
    }

    void children_returnsCorrectList()
    {
        AbstractModelItem parent;
        AbstractModelItem* c1 = new AbstractModelItem();
        AbstractModelItem* c2 = new AbstractModelItem();
        parent.appendChild(c1);
        parent.appendChild(c2);

        AbstractModelItem::List kids = parent.children();
        QCOMPARE(kids.count(), 2);
        QCOMPARE(kids.at(0), c1);
        QCOMPARE(kids.at(1), c2);
    }
};

QTEST_MAIN(TstAbstractModelItem)
#include "tst_abstractmodelitem.moc"
