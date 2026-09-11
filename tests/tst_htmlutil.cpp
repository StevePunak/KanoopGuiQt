#include <QTest>
#include <QColor>
#include <QRegularExpression>
#include <QString>

#include <Kanoop/gui/utility/htmlutil.h>

namespace
{

/**
 * @brief Return the value of one declaration from a style attribute.
 *
 * "background-color" contains "color", so a substring search for the foreground
 * matches markup that carries only the background. The lookbehind separates them.
 *
 * @param markup Markup to search
 * @param property Property name, e.g. "color" or "background-color"
 * @return The declared value, or an empty string when the property is not declared
 */
QString declaredValue(const QString& markup, const QString& property)
{
    const QString pattern = QStringLiteral("(?<![-\\w])") + QRegularExpression::escape(property)
                            + QStringLiteral("\\s*:\\s*([^;\"]+)");
    const QRegularExpressionMatch match = QRegularExpression(pattern).match(markup);
    return match.hasMatch() ? match.captured(1).trimmed() : QString();
}

bool declaredColourIs(const QString& markup, const QString& property, const QColor& expected)
{
    const QString value = declaredValue(markup, property);
    if(value.isEmpty()) {
        return false;
    }
    return QColor(value).name(QColor::HexArgb) == expected.name(QColor::HexArgb);
}

const QColor Foreground = QColor(255, 0, 0);
const QColor Background = QColor(255, 215, 0);

}

class TstHtmlUtil : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief The declaration extractor does not confuse the two colour properties.
     *
     * Every assertion below is only as good as this helper.
     */
    void declaredValue_separatesColourFromBackgroundColor()
    {
        const QString backgroundOnly = QStringLiteral("<p style=\"background-color: #ffffd700;\">");
        QCOMPARE(declaredValue(backgroundOnly, QStringLiteral("background-color")),
                 QStringLiteral("#ffffd700"));
        QVERIFY2(declaredValue(backgroundOnly, QStringLiteral("color")).isEmpty(),
                 "the foreground lookup matched the background declaration");

        const QString both = QStringLiteral("<p style=\"color: #ffff0000;background-color: #ffffd700;\">");
        QCOMPARE(declaredValue(both, QStringLiteral("color")), QStringLiteral("#ffff0000"));
        QCOMPARE(declaredValue(both, QStringLiteral("background-color")), QStringLiteral("#ffffd700"));
    }

    /**
     * @brief A background colour is emitted even when the foreground is invalid.
     */
    void startParagraph_backgroundOnly_emitsTheBackground()
    {
        const QString markup = HtmlUtil::startParagraph(QColor(), Background);

        QVERIFY2(declaredColourIs(markup, QStringLiteral("background-color"), Background),
                 qPrintable(QStringLiteral("markup was: ") + markup));
        QVERIFY2(declaredValue(markup, QStringLiteral("color")).isEmpty(),
                 qPrintable(QStringLiteral("markup was: ") + markup));
    }

    /**
     * @brief Both colours are still emitted when both are valid.
     *
     * Control for the case above: a version that satisfied it by dropping the
     * foreground declaration entirely would fail here.
     */
    void startParagraph_bothValid_emitsBoth()
    {
        const QString markup = HtmlUtil::startParagraph(Foreground, Background);

        QVERIFY2(declaredColourIs(markup, QStringLiteral("color"), Foreground),
                 qPrintable(QStringLiteral("markup was: ") + markup));
        QVERIFY2(declaredColourIs(markup, QStringLiteral("background-color"), Background),
                 qPrintable(QStringLiteral("markup was: ") + markup));
    }

    /**
     * @brief A foreground colour alone is emitted alone.
     */
    void startParagraph_foregroundOnly_emitsTheForeground()
    {
        const QString markup = HtmlUtil::startParagraph(Foreground, QColor());

        QVERIFY2(declaredColourIs(markup, QStringLiteral("color"), Foreground),
                 qPrintable(QStringLiteral("markup was: ") + markup));
        QVERIFY2(declaredValue(markup, QStringLiteral("background-color")).isEmpty(),
                 qPrintable(QStringLiteral("markup was: ") + markup));
    }

    /**
     * @brief Two invalid colours emit no style attribute at all.
     */
    void startParagraph_neitherValid_emitsABareParagraph()
    {
        QCOMPARE(HtmlUtil::startParagraph(QColor(), QColor()), QStringLiteral("<p>"));
        QCOMPARE(HtmlUtil::startParagraph(), QStringLiteral("<p>"));
    }
};

QTEST_MAIN(TstHtmlUtil)
#include "tst_htmlutil.moc"
