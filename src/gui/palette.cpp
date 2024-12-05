#include "palette.h"

#include <QMetaEnum>
#include <Kanoop/log.h>

Palette Palette::fusionLight()
{
    const QColor windowTextColor = Qt::black;
    const QColor backGroundColor = QColor(239, 239, 239);
    const QColor lightColor = backGroundColor.lighter(150);
    const QColor midColor = (backGroundColor.darker(130));
    const QColor midLightColor = midColor.lighter(110);
    const QColor baseColor = Qt::white;
    const QColor disabledBaseColor = backGroundColor;
    const QColor darkColor = backGroundColor.darker(150);
    const QColor darkDisabledColor = QColor(209, 209, 209).darker(110);
    const QColor textColor = Qt::black;
    const QColor highlightColor = QColor(48, 140, 198);
    const QColor hightlightedTextColor = Qt::white;
    const QColor disabledTextColor = QColor(190, 190, 190);
    const QColor buttonColor = backGroundColor;
    const QColor shadowColor = darkColor.darker(135);
    const QColor disabledShadowColor = shadowColor.lighter(150);
    const QColor disabledHighlightColor(145, 145, 145);
    QColor placeholderColor = textColor;
    placeholderColor.setAlpha(128);

    Palette palette(windowTextColor, backGroundColor, lightColor, darkColor, midColor, textColor, baseColor);
    palette.setBrush(QPalette::Midlight, midLightColor);
    palette.setBrush(QPalette::Button, buttonColor);
    palette.setBrush(QPalette::Shadow, shadowColor);
    palette.setBrush(QPalette::HighlightedText, hightlightedTextColor);

    palette.setBrush(QPalette::Disabled, QPalette::Text, disabledTextColor);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, disabledTextColor);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, disabledTextColor);
    palette.setBrush(QPalette::Disabled, QPalette::Base, disabledBaseColor);
    palette.setBrush(QPalette::Disabled, QPalette::Dark, darkDisabledColor);
    palette.setBrush(QPalette::Disabled, QPalette::Shadow, disabledShadowColor);

    palette.setBrush(QPalette::Active, QPalette::Highlight, highlightColor);
    palette.setBrush(QPalette::Inactive, QPalette::Highlight, highlightColor);
    palette.setBrush(QPalette::Disabled, QPalette::Highlight, disabledHighlightColor);

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
    palette.setBrush(QPalette::Active, QPalette::Accent, highlightColor);
    palette.setBrush(QPalette::Inactive, QPalette::Accent, highlightColor);
    palette.setBrush(QPalette::Disabled, QPalette::Accent, disabledHighlightColor);
#endif
    palette.setBrush(QPalette::PlaceholderText, placeholderColor);

    return palette;
}

Palette Palette::fusionDark()
{
    const QColor windowTextColor = QColor(240, 240, 240);
    const QColor backGroundColor = QColor(50, 50, 50);
    const QColor lightColor = backGroundColor.lighter(150);
    const QColor midColor = (backGroundColor.darker(130));
    const QColor midLightColor = midColor.lighter(110);
    const QColor baseColor = backGroundColor.darker(140);
    const QColor disabledBaseColor = backGroundColor;
    const QColor darkColor = backGroundColor.darker(150);
    const QColor darkDisabledColor = QColor(209, 209, 209).darker(110);
    const QColor textColor = windowTextColor;
    const QColor highlightColor = QColor(48, 140, 198);
    const QColor hightlightedTextColor = windowTextColor;
    const QColor disabledTextColor = QColor(130, 130, 130);
    const QColor buttonColor = backGroundColor;
    const QColor shadowColor = darkColor.darker(135);
    const QColor disabledShadowColor = shadowColor.lighter(150);
    const QColor disabledHighlightColor(145, 145, 145);
    QColor placeholderColor = textColor;
    placeholderColor.setAlpha(128);

    Palette palette(windowTextColor, backGroundColor, lightColor, darkColor, midColor, textColor, baseColor);
    palette.setBrush(QPalette::Midlight, midLightColor);
    palette.setBrush(QPalette::Button, buttonColor);
    palette.setBrush(QPalette::Shadow, shadowColor);
    palette.setBrush(QPalette::HighlightedText, hightlightedTextColor);

    palette.setBrush(QPalette::Disabled, QPalette::Text, disabledTextColor);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, disabledTextColor);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, disabledTextColor);
    palette.setBrush(QPalette::Disabled, QPalette::Base, disabledBaseColor);
    palette.setBrush(QPalette::Disabled, QPalette::Dark, darkDisabledColor);
    palette.setBrush(QPalette::Disabled, QPalette::Shadow, disabledShadowColor);

    palette.setBrush(QPalette::Active, QPalette::Highlight, highlightColor);
    palette.setBrush(QPalette::Inactive, QPalette::Highlight, highlightColor);
    palette.setBrush(QPalette::Disabled, QPalette::Highlight, disabledHighlightColor);

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
    palette.setBrush(QPalette::Active, QPalette::Accent, highlightColor);
    palette.setBrush(QPalette::Inactive, QPalette::Accent, highlightColor);
    palette.setBrush(QPalette::Disabled, QPalette::Accent, disabledHighlightColor);
#endif
    palette.setBrush(QPalette::PlaceholderText, placeholderColor);

    palette.setBrush(QPalette::Link, highlightColor);
    return palette;
}

void Palette::debugDumpPalette(const Palette& palette)
{
    static const QList<ColorRole> roles = {
        Accent,
        AlternateBase,
        Base,
        BrightText,
        Button,
        ButtonText,
        Dark,
        Highlight,
        HighlightedText,
        Light,
        Link,
        LinkVisited,
        Mid,
        Midlight,
        NoRole,
        PlaceholderText,
        Shadow,
        Text,
        ToolTipBase, ToolTipText,
        Window,
        WindowText,
    };

    QMetaEnum metaEnum = QMetaEnum::fromType<ColorRole>();
    for(ColorRole role : roles) {
        Log::logText(LVL_DEBUG, QString("%1: 0x%2").arg(QString(metaEnum.valueToKey(role)).leftJustified(32)).arg(palette.color(role).name()));
    }
}
