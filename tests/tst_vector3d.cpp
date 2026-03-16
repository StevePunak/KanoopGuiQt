#include <QTest>
#include <Kanoop/gui/vector3d.h>

class TstVector3D : public QObject
{
    Q_OBJECT

private slots:
    void defaultConstructor_isZero()
    {
        Vector3D v;
        QCOMPARE(v.x(), 0.0f);
        QCOMPARE(v.y(), 0.0f);
        QCOMPARE(v.z(), 0.0f);
    }

    void componentConstructor_setsValues()
    {
        Vector3D v(1.5f, 2.5f, 3.5f);
        QCOMPARE(v.x(), 1.5f);
        QCOMPARE(v.y(), 2.5f);
        QCOMPARE(v.z(), 3.5f);
    }

    void toString_formatsCorrectly()
    {
        Vector3D v(1.0f, 2.0f, 3.0f);
        QString s = v.toString();
        QVERIFY(s.contains("1"));
        QVERIFY(s.contains("2"));
        QVERIFY(s.contains("3"));
    }

    void fromString_parsesCorrectly()
    {
        Vector3D v = Vector3D::fromString("1.5, 2.5, 3.5");
        QCOMPARE(v.x(), 1.5f);
        QCOMPARE(v.y(), 2.5f);
        QCOMPARE(v.z(), 3.5f);
    }

    void fromString_roundTrip()
    {
        Vector3D original(4.0f, 5.0f, 6.0f);
        Vector3D parsed = Vector3D::fromString(original.toString());
        QCOMPARE(parsed.x(), original.x());
        QCOMPARE(parsed.y(), original.y());
        QCOMPARE(parsed.z(), original.z());
    }

    void fromString_invalidInput_returnsZero()
    {
        Vector3D v = Vector3D::fromString("invalid");
        QCOMPARE(v.x(), 0.0f);
        QCOMPARE(v.y(), 0.0f);
        QCOMPARE(v.z(), 0.0f);
    }

    void fromString_tooFewParts_returnsZero()
    {
        Vector3D v = Vector3D::fromString("1.0, 2.0");
        QCOMPARE(v.x(), 0.0f);
        QCOMPARE(v.y(), 0.0f);
        QCOMPARE(v.z(), 0.0f);
    }

    void fromString_emptyString_returnsZero()
    {
        Vector3D v = Vector3D::fromString("");
        QCOMPARE(v.x(), 0.0f);
        QCOMPARE(v.y(), 0.0f);
        QCOMPARE(v.z(), 0.0f);
    }

    void fromString_negativeValues()
    {
        Vector3D v = Vector3D::fromString("-1.0, -2.0, -3.0");
        QCOMPARE(v.x(), -1.0f);
        QCOMPARE(v.y(), -2.0f);
        QCOMPARE(v.z(), -3.0f);
    }

    void fromString_extraSpaces()
    {
        Vector3D v = Vector3D::fromString("  1.0 ,  2.0 ,  3.0  ");
        QCOMPARE(v.x(), 1.0f);
        QCOMPARE(v.y(), 2.0f);
        QCOMPARE(v.z(), 3.0f);
    }

    void pointConstructor_setsXYZeroZ()
    {
        Vector3D v(QPoint(10, 20));
        QCOMPARE(v.x(), 10.0f);
        QCOMPARE(v.y(), 20.0f);
        QCOMPARE(v.z(), 0.0f);
    }
};

QTEST_MAIN(TstVector3D)
#include "tst_vector3d.moc"
