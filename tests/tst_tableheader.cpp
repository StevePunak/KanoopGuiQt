#include <QTest>
#include <Kanoop/gui/tableheader.h>

class TstTableHeader : public QObject
{
    Q_OBJECT

private slots:
    void defaultConstructor_isInvalid()
    {
        TableHeader h;
        QVERIFY(!h.isValid());
        QCOMPARE(h.type(), 0);
        QCOMPARE(h.orientation(), Qt::Horizontal);
    }

    void typeConstructor_setsType()
    {
        TableHeader h(42);
        QVERIFY(h.isValid());
        QCOMPARE(h.type(), 42);
        QCOMPARE(h.orientation(), Qt::Vertical);
    }

    void typeTextConstructor_setsTypeAndText()
    {
        TableHeader h(1, "Name");
        QVERIFY(h.isValid());
        QCOMPARE(h.type(), 1);
        QCOMPARE(h.text(), QStringLiteral("Name"));
        QCOMPARE(h.orientation(), Qt::Vertical);
    }

    void orientationOverride()
    {
        TableHeader h(1, "Col", Qt::Horizontal);
        QCOMPARE(h.orientation(), Qt::Horizontal);
    }

    void setText_modifiesText()
    {
        TableHeader h(1, "Old");
        h.setText("New");
        QCOMPARE(h.text(), QStringLiteral("New"));
    }

    void visibility_defaultTrue()
    {
        TableHeader h(1, "Test");
        QVERIFY(h.isVisible());
    }

    void setVisible_works()
    {
        TableHeader h(1, "Test");
        h.setVisible(false);
        QVERIFY(!h.isVisible());
        h.setVisible(true);
        QVERIFY(h.isVisible());
    }

    void columnTextColor_defaultInvalid()
    {
        TableHeader h(1);
        QVERIFY(!h.columnTextColor().isValid());
    }

    void setColumnTextColor_works()
    {
        TableHeader h(1);
        h.setColumnTextColor(Qt::red);
        QCOMPARE(h.columnTextColor(), QColor(Qt::red));
    }

    // --- TableHeader::List tests ---

    void list_findByName_found()
    {
        TableHeader::List list;
        list.append(TableHeader(1, "Alpha"));
        list.append(TableHeader(2, "Beta"));
        list.append(TableHeader(3, "Gamma"));

        TableHeader found = list.findByName("Beta");
        QVERIFY(found.isValid());
        QCOMPARE(found.type(), 2);
    }

    void list_findByName_notFound()
    {
        TableHeader::List list;
        list.append(TableHeader(1, "Alpha"));

        TableHeader found = list.findByName("Nonexistent");
        QVERIFY(!found.isValid());
    }

    void list_setHeaderVisible()
    {
        TableHeader::List list;
        list.append(TableHeader(1, "A"));
        list.append(TableHeader(2, "B"));

        list.setHeaderVisible(2, false);

        // Find the header with type 2 and check visibility
        TableHeader found = list.findByName("B");
        QVERIFY(!found.isVisible());
    }

    // --- TableHeader::IntMap tests ---

    void intMap_toSortedList_sortsByKey()
    {
        TableHeader::IntMap map;
        map.insert(3, TableHeader(30, "Third"));
        map.insert(1, TableHeader(10, "First"));
        map.insert(2, TableHeader(20, "Second"));

        TableHeader::List sorted = map.toSortedList();
        QCOMPARE(sorted.count(), 3);
        QCOMPARE(sorted.at(0).type(), 10);
        QCOMPARE(sorted.at(1).type(), 20);
        QCOMPARE(sorted.at(2).type(), 30);
    }

    void intMap_setTextColorForType()
    {
        TableHeader::IntMap map;
        map.insert(0, TableHeader(1, "Col"));

        map.setTextColorForType(1, Qt::blue);

        QCOMPARE(map.value(0).columnTextColor(), QColor(Qt::blue));
    }

    void intMap_setHeaderVisible()
    {
        TableHeader::IntMap map;
        map.insert(0, TableHeader(5, "Col"));
        QVERIFY(map.value(0).isVisible());

        map.setHeaderVisible(5, false);
        QVERIFY(!map.value(0).isVisible());
    }

    void intMap_setEntityMetadataForType()
    {
        TableHeader::IntMap map;
        map.insert(0, TableHeader(1, "Col"));

        EntityMetadata meta(42);
        map.setEntityMetadataForType(1, meta);

        QCOMPARE(map.value(0).entityMetadata().type(), 42);
    }
};

QTEST_MAIN(TstTableHeader)
#include "tst_tableheader.moc"
