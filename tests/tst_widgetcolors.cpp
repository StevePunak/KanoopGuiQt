#include <QTest>
#include <QColor>
#include <QRegularExpression>
#include <QString>

#include <Kanoop/gui/widgets/frame.h>
#include <Kanoop/gui/widgets/lineedit.h>

namespace
{

/**
 * @brief Return the value of one declaration from a stylesheet.
 *
 * "background-color" contains "color", so a substring search for the foreground
 * property matches the background declaration and reports success on a stylesheet
 * that carries only the background. The lookbehind is what separates them.
 *
 * @param sheet Stylesheet text
 * @param property Property name, e.g. "color" or "background-color"
 * @return The declared value with surrounding whitespace removed, or an empty
 *         string when the property is not declared
 */
QString declaredValue(const QString& sheet, const QString& property)
{
    const QString pattern = QStringLiteral("(?<![-\\w])") + QRegularExpression::escape(property)
                            + QStringLiteral("\\s*:\\s*([^;}]+)");
    const QRegularExpressionMatch match = QRegularExpression(pattern).match(sheet);
    return match.hasMatch() ? match.captured(1).trimmed() : QString();
}

/**
 * @brief Compare a declared colour against an expected one.
 *
 * Both sides are normalised through QColor so the assertion does not depend on the
 * notation the stylesheet writer happens to emit.
 */
bool declaredColourIs(const QString& sheet, const QString& property, const QColor& expected)
{
    const QString value = declaredValue(sheet, property);
    if(value.isEmpty()) {
        return false;
    }
    return QColor(value).name(QColor::HexArgb) == expected.name(QColor::HexArgb);
}

QString describe(const QString& sheet)
{
    return QStringLiteral("stylesheet was: ") + sheet;
}

const QColor Foreground = QColor(255, 0, 0);
const QColor Background = QColor(255, 215, 0);

}

class TstWidgetColors : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief The declaration extractor does not confuse the two colour properties.
     *
     * Every assertion below is only as good as this helper. On a sheet carrying the
     * background alone, asking for "color" must come back empty.
     */
    void declaredValue_separatesColourFromBackgroundColor()
    {
        const QString backgroundOnly = QStringLiteral("QFrame { background-color: #ffffd700 }");
        QCOMPARE(declaredValue(backgroundOnly, QStringLiteral("background-color")),
                 QStringLiteral("#ffffd700"));
        QVERIFY2(declaredValue(backgroundOnly, QStringLiteral("color")).isEmpty(),
                 "the foreground lookup matched the background declaration");

        const QString foregroundOnly = QStringLiteral("QFrame { color: #ffff0000 }");
        QCOMPARE(declaredValue(foregroundOnly, QStringLiteral("color")), QStringLiteral("#ffff0000"));
        QVERIFY(declaredValue(foregroundOnly, QStringLiteral("background-color")).isEmpty());

        const QString both = QStringLiteral("QFrame { background-color: #ffffd700; color: #ffff0000 }");
        QCOMPARE(declaredValue(both, QStringLiteral("color")), QStringLiteral("#ffff0000"));
        QCOMPARE(declaredValue(both, QStringLiteral("background-color")), QStringLiteral("#ffffd700"));
    }

    void lineEdit_foregroundThenBackground_keepsBoth()
    {
        LineEdit widget;
        widget.setForegroundColor(Foreground);
        widget.setBackgroundColor(Background);

        const QString sheet = widget.styleSheet();
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("color"), Foreground), qPrintable(describe(sheet)));
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("background-color"), Background), qPrintable(describe(sheet)));
    }

    void lineEdit_backgroundThenForeground_keepsBoth()
    {
        LineEdit widget;
        widget.setBackgroundColor(Background);
        widget.setForegroundColor(Foreground);

        const QString sheet = widget.styleSheet();
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("color"), Foreground), qPrintable(describe(sheet)));
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("background-color"), Background), qPrintable(describe(sheet)));
    }

    void frame_foregroundThenBackground_keepsBoth()
    {
        Frame widget;
        widget.setForegroundColor(Foreground);
        widget.setBackgroundColor(Background);

        const QString sheet = widget.styleSheet();
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("color"), Foreground), qPrintable(describe(sheet)));
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("background-color"), Background), qPrintable(describe(sheet)));
    }

    void frame_backgroundThenForeground_keepsBoth()
    {
        Frame widget;
        widget.setBackgroundColor(Background);
        widget.setForegroundColor(Foreground);

        const QString sheet = widget.styleSheet();
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("color"), Foreground), qPrintable(describe(sheet)));
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("background-color"), Background), qPrintable(describe(sheet)));
    }

    /**
     * @brief Frame's Q_PROPERTY getters agree with what the widget paints.
     *
     * The getter returned a stored colour that the stylesheet no longer carried, so
     * it could report a foreground the widget was not painting.
     */
    void frame_gettersAgreeWithTheStylesheet()
    {
        Frame widget;
        widget.setForegroundColor(Foreground);
        widget.setBackgroundColor(Background);

        QCOMPARE(widget.foregroundColor().name(QColor::HexArgb), Foreground.name(QColor::HexArgb));
        QCOMPARE(widget.backgroundColor().name(QColor::HexArgb), Background.name(QColor::HexArgb));

        const QString sheet = widget.styleSheet();
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("color"), widget.foregroundColor()),
                 qPrintable(describe(sheet)));
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("background-color"), widget.backgroundColor()),
                 qPrintable(describe(sheet)));
    }

    /**
     * @brief Clearing one colour leaves the other in place.
     */
    void setDefaultForegroundColor_dropsOneAndKeepsTheOther()
    {
        LineEdit lineEdit;
        lineEdit.setForegroundColor(Foreground);
        lineEdit.setBackgroundColor(Background);
        lineEdit.setDefaultForegroundColor();

        QString sheet = lineEdit.styleSheet();
        QVERIFY2(declaredValue(sheet, QStringLiteral("color")).isEmpty(), qPrintable(describe(sheet)));
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("background-color"), Background), qPrintable(describe(sheet)));

        Frame frame;
        frame.setForegroundColor(Foreground);
        frame.setBackgroundColor(Background);
        frame.setDefaultForegroundColor();

        sheet = frame.styleSheet();
        QVERIFY2(declaredValue(sheet, QStringLiteral("color")).isEmpty(), qPrintable(describe(sheet)));
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("background-color"), Background), qPrintable(describe(sheet)));
    }

    void setDefaultBackgroundColor_dropsOneAndKeepsTheOther()
    {
        LineEdit lineEdit;
        lineEdit.setForegroundColor(Foreground);
        lineEdit.setBackgroundColor(Background);
        lineEdit.setDefaultBackgroundColor();

        QString sheet = lineEdit.styleSheet();
        QVERIFY2(declaredValue(sheet, QStringLiteral("background-color")).isEmpty(), qPrintable(describe(sheet)));
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("color"), Foreground), qPrintable(describe(sheet)));

        Frame frame;
        frame.setForegroundColor(Foreground);
        frame.setBackgroundColor(Background);
        frame.setDefaultBackgroundColor();

        sheet = frame.styleSheet();
        QVERIFY2(declaredValue(sheet, QStringLiteral("background-color")).isEmpty(), qPrintable(describe(sheet)));
        QVERIFY2(declaredColourIs(sheet, QStringLiteral("color"), Foreground), qPrintable(describe(sheet)));
    }
};

QTEST_MAIN(TstWidgetColors)
#include "tst_widgetcolors.moc"
