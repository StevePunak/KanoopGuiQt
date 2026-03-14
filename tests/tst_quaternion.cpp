#include <QTest>
#include <Kanoop/gui/quaternion.h>

class TstQuaternion : public QObject
{
    Q_OBJECT

private slots:
    void defaultConstructor_isIdentity()
    {
        Quaternion q;
        QCOMPARE(q.scalar(), 1.0f);
        QCOMPARE(q.x(), 0.0f);
        QCOMPARE(q.y(), 0.0f);
        QCOMPARE(q.z(), 0.0f);
    }

    void componentConstructor_setsValues()
    {
        Quaternion q(0.5f, 1.0f, 2.0f, 3.0f);
        QCOMPARE(q.scalar(), 0.5f);
        QCOMPARE(q.x(), 1.0f);
        QCOMPARE(q.y(), 2.0f);
        QCOMPARE(q.z(), 3.0f);
    }

    void toString_formatsCorrectly()
    {
        Quaternion q(0.5f, 1.0f, 2.0f, 3.0f);
        QString s = q.toString();
        // Format is "x, y, z, scalar"
        QVERIFY(s.contains("1"));
        QVERIFY(s.contains("2"));
        QVERIFY(s.contains("3"));
        QVERIFY(s.contains("0.5"));
    }

    void fromString_parsesCorrectly()
    {
        // Format: "x, y, z, scalar"
        Quaternion q = Quaternion::fromString("1.0, 2.0, 3.0, 0.5");
        QCOMPARE(q.x(), 1.0f);
        QCOMPARE(q.y(), 2.0f);
        QCOMPARE(q.z(), 3.0f);
        QCOMPARE(q.scalar(), 0.5f);
    }

    void fromString_roundTrip()
    {
        Quaternion original(0.7f, 0.1f, 0.2f, 0.3f);
        Quaternion parsed = Quaternion::fromString(original.toString());
        QCOMPARE(parsed.x(), original.x());
        QCOMPARE(parsed.y(), original.y());
        QCOMPARE(parsed.z(), original.z());
        QCOMPARE(parsed.scalar(), original.scalar());
    }

    void fromString_invalidInput_returnsIdentity()
    {
        Quaternion q = Quaternion::fromString("invalid");
        QCOMPARE(q.scalar(), 1.0f);
        QCOMPARE(q.x(), 0.0f);
        QCOMPARE(q.y(), 0.0f);
        QCOMPARE(q.z(), 0.0f);
    }

    void fromString_tooFewParts_returnsIdentity()
    {
        Quaternion q = Quaternion::fromString("1.0, 2.0, 3.0");
        QCOMPARE(q.scalar(), 1.0f);
        QCOMPARE(q.x(), 0.0f);
    }

    void fromString_emptyString_returnsIdentity()
    {
        Quaternion q = Quaternion::fromString("");
        QCOMPARE(q.scalar(), 1.0f);
    }

    void fromString_negativeValues()
    {
        Quaternion q = Quaternion::fromString("-1.0, -2.0, -3.0, -0.5");
        QCOMPARE(q.x(), -1.0f);
        QCOMPARE(q.y(), -2.0f);
        QCOMPARE(q.z(), -3.0f);
        QCOMPARE(q.scalar(), -0.5f);
    }
};

QTEST_MAIN(TstQuaternion)
#include "tst_quaternion.moc"
