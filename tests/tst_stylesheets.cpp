#include <QTest>
#include <Kanoop/gui/stylesheets.h>

class TstStyleSheets : public QObject
{
    Q_OBJECT

private slots:
    void backgroundColor_formatsRgb()
    {
        QString result = StyleSheets::backgroundColor(QColor(85, 170, 255));
        QCOMPARE(result, QStringLiteral("background-color: rgb(85, 170, 255);"));
    }

    void backgroundColor_black()
    {
        QString result = StyleSheets::backgroundColor(QColor(0, 0, 0));
        QCOMPARE(result, QStringLiteral("background-color: rgb(0, 0, 0);"));
    }

    void backgroundColor_white()
    {
        QString result = StyleSheets::backgroundColor(QColor(255, 255, 255));
        QCOMPARE(result, QStringLiteral("background-color: rgb(255, 255, 255);"));
    }

    void foregroundColor_formatsRgb()
    {
        QString result = StyleSheets::foregroundColor(QColor(100, 200, 50));
        QCOMPARE(result, QStringLiteral("color: rgb(100, 200, 50);"));
    }

    void borderRadius_formatsPixels()
    {
        QString result = StyleSheets::borderRadius(4);
        QVERIFY(result.contains("border-radius"));
        QVERIFY(result.contains("4"));
    }

    void borderRadius_zero()
    {
        QString result = StyleSheets::borderRadius(0);
        QVERIFY(result.contains("0"));
    }
};

QTEST_MAIN(TstStyleSheets)
#include "tst_stylesheets.moc"
