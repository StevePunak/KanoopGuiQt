#include <QTest>
#include <Kanoop/gui/headerstate.h>

class TstHeaderState : public QObject
{
    Q_OBJECT

private slots:
    void addSection_getSection_roundTrip()
    {
        HeaderState state;
        state.addSection(0, "Name", 150, true);
        state.addSection(1, "Value", 200, false);

        HeaderState::SectionState s0 = state.getSection(0);
        QVERIFY(s0.isValid());
        QCOMPARE(s0.section(), 0);
        QCOMPARE(s0.text(), QStringLiteral("Name"));
        QCOMPARE(s0.size(), 150);
        QVERIFY(s0.isVisible());

        HeaderState::SectionState s1 = state.getSection(1);
        QVERIFY(s1.isValid());
        QCOMPARE(s1.section(), 1);
        QCOMPARE(s1.text(), QStringLiteral("Value"));
        QCOMPARE(s1.size(), 200);
        QVERIFY(!s1.isVisible());
    }

    void getSection_nonExistent_returnsInvalid()
    {
        HeaderState state;
        state.addSection(0, "Name", 100, true);

        HeaderState::SectionState s = state.getSection(99);
        QVERIFY(!s.isValid());
    }

    void serializeDeserialize_roundTrip()
    {
        HeaderState original;
        original.addSection(0, "Alpha", 100, true);
        original.addSection(1, "Beta", 200, false);
        original.addSection(2, "Gamma", 300, true);

        QByteArray json = original.serializeToJson();
        QVERIFY(!json.isEmpty());

        HeaderState restored;
        restored.deserializeFromJson(json);

        HeaderState::SectionState s0 = restored.getSection(0);
        QVERIFY(s0.isValid());
        QCOMPARE(s0.text(), QStringLiteral("Alpha"));
        QCOMPARE(s0.size(), 100);
        QVERIFY(s0.isVisible());

        HeaderState::SectionState s1 = restored.getSection(1);
        QVERIFY(s1.isValid());
        QCOMPARE(s1.text(), QStringLiteral("Beta"));
        QCOMPARE(s1.size(), 200);
        QVERIFY(!s1.isVisible());

        HeaderState::SectionState s2 = restored.getSection(2);
        QVERIFY(s2.isValid());
        QCOMPARE(s2.text(), QStringLiteral("Gamma"));
        QCOMPARE(s2.size(), 300);
        QVERIFY(s2.isVisible());
    }

    void sectionState_defaultConstructor_isInvalid()
    {
        HeaderState::SectionState s;
        QVERIFY(!s.isValid());
        QCOMPARE(s.section(), 0);
        QCOMPARE(s.size(), 100);
        QVERIFY(s.isVisible());
    }

    void sectionState_fullConstructor()
    {
        HeaderState::SectionState s(5, "Column", 250, false);
        QVERIFY(s.isValid());
        QCOMPARE(s.section(), 5);
        QCOMPARE(s.text(), QStringLiteral("Column"));
        QCOMPARE(s.size(), 250);
        QVERIFY(!s.isVisible());
    }

    void sectionState_jsonObjectRoundTrip()
    {
        HeaderState::SectionState original(3, "Test", 175, true);
        QJsonObject obj = original.serializeToJsonObject();

        HeaderState::SectionState restored;
        restored.deserializeFromJsonObject(obj);

        QCOMPARE(restored.section(), 3);
        QCOMPARE(restored.text(), QStringLiteral("Test"));
        QCOMPARE(restored.size(), 175);
        QVERIFY(restored.isVisible());
    }
};

QTEST_MAIN(TstHeaderState)
#include "tst_headerstate.moc"
