#include <QTest>
#include <QApplication>
#include <Kanoop/gui/palette.h>

class TstPalette : public QObject
{
    Q_OBJECT

private slots:
    void fusionLight_returnsValidPalette()
    {
        Palette p = Palette::fusionLight();
        // Should have a white base
        QCOMPARE(p.color(QPalette::Base), QColor(Qt::white));
        // Window text should be black
        QCOMPARE(p.color(QPalette::WindowText), QColor(Qt::black));
    }

    void fusionDark_returnsValidPalette()
    {
        Palette p = Palette::fusionDark();
        // Dark palette base should be darker than light
        Palette light = Palette::fusionLight();
        QVERIFY(p.color(QPalette::Base).lightness() < light.color(QPalette::Base).lightness());
    }

    void fusionDark_windowText_isLight()
    {
        Palette p = Palette::fusionDark();
        QCOMPARE(p.color(QPalette::WindowText), QColor(240, 240, 240));
    }

    void toVariant_fromVariant_roundTrip()
    {
        Palette original = Palette::fusionLight();
        QVariant v = original.toVariant();
        Palette restored = Palette::fromVariant(v);
        QCOMPARE(restored.color(QPalette::Base), original.color(QPalette::Base));
        QCOMPARE(restored.color(QPalette::WindowText), original.color(QPalette::WindowText));
        QCOMPARE(restored.color(QPalette::Button), original.color(QPalette::Button));
    }

    void getColorRoleString_knownRoles()
    {
        QCOMPARE(Palette::getColorRoleString(QPalette::WindowText), QStringLiteral("WindowText"));
        QCOMPARE(Palette::getColorRoleString(QPalette::Button), QStringLiteral("Button"));
        QCOMPARE(Palette::getColorRoleString(QPalette::Base), QStringLiteral("Base"));
        QCOMPARE(Palette::getColorRoleString(QPalette::Text), QStringLiteral("Text"));
        QCOMPARE(Palette::getColorRoleString(QPalette::Highlight), QStringLiteral("Highlight"));
        QCOMPARE(Palette::getColorRoleString(QPalette::Shadow), QStringLiteral("Shadow"));
    }

    void getColorRole_knownStrings()
    {
        QCOMPARE(Palette::getColorRole("WindowText"), QPalette::WindowText);
        QCOMPARE(Palette::getColorRole("Button"), QPalette::Button);
        QCOMPARE(Palette::getColorRole("Base"), QPalette::Base);
        QCOMPARE(Palette::getColorRole("Highlight"), QPalette::Highlight);
    }

    void getColorRoleStrings_containsExpected()
    {
        QStringList roles = Palette::getColorRoleStrings();
        QVERIFY(roles.contains("WindowText"));
        QVERIFY(roles.contains("Button"));
        QVERIFY(roles.contains("Base"));
        QVERIFY(roles.contains("Text"));
        QVERIFY(roles.contains("NoRole"));
    }

    void getColorRoleString_roundTrip()
    {
        QPalette::ColorRole role = QPalette::HighlightedText;
        QString str = Palette::getColorRoleString(role);
        QPalette::ColorRole back = Palette::getColorRole(str);
        QCOMPARE(back, role);
    }

    void copyFromQPalette()
    {
        QPalette qp;
        qp.setColor(QPalette::Base, Qt::cyan);
        Palette p(qp);
        QCOMPARE(p.color(QPalette::Base), QColor(Qt::cyan));
    }

    void defaultConstructor_isValid()
    {
        Palette p;
        // Should not crash; palette should be usable
        QColor c = p.color(QPalette::Base);
        Q_UNUSED(c)
    }
};

QTEST_MAIN(TstPalette)
#include "tst_palette.moc"
