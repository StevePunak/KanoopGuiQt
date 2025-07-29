#ifndef PALETTE_H
#define PALETTE_H
#include <QPalette>
#include <QVariant>
#include <Kanoop/gui/libkanoopgui.h>
#include <Kanoop/kanoopcommon.h>

class LIBKANOOPGUI_EXPORT Palette : public QPalette
{
public:
    Palette() :
        QPalette() {}
    Palette(const QColor &button) :
        QPalette(button) {}
    Palette(Qt::GlobalColor button) :
        QPalette(button) {}
    Palette(const QColor &button, const QColor &window) :
        QPalette(button, window) {}
    Palette(const QBrush &windowText, const QBrush &button, const QBrush &light,
             const QBrush &dark, const QBrush &mid, const QBrush &text,
             const QBrush &bright_text, const QBrush &base, const QBrush &window) :
        QPalette(windowText, button, light, dark, mid, text, bright_text, base, window) {}
    Palette(const QColor &windowText, const QColor &window, const QColor &light,
             const QColor &dark, const QColor &mid, const QColor &text, const QColor &base) :
        QPalette(windowText, window, light, dark, mid, text, base) {}
    Palette(const QPalette &palette) :
        QPalette(palette) {}

    QVariant toVariant() const { return QVariant::fromValue<Palette>(*this); }
    static Palette fromVariant(const QVariant& value) { return value.value<Palette>(); }

    class List : public QList<Palette> {};

    static Palette fusionLight();
    static Palette fusionDark();

    static void debugDumpPalette(const Palette& palette);

    static QString getColorRoleString(ColorRole role) { return _ColorRoleToStringMap.getString(role); }
    static QStringList getColorRoleStrings() { return _ColorRoleToStringMap.values(); }
    static ColorRole getColorRole(const QString& value) { return _ColorRoleToStringMap.getType(value); }

private:
    class ColorGroupToStringMap : public KANOOP::EnumToStringMap<QPalette::ColorGroup>
    {
    public:
        ColorGroupToStringMap()
        {
            insert(QPalette::Active,            "Active");
            insert(QPalette::Disabled,          "Disabled");
            insert(QPalette::Inactive,          "Inactive");
        }
    };

    class ColorRoleToStringMap : public KANOOP::EnumToStringMap<QPalette::ColorRole>
    {
    public:
        ColorRoleToStringMap()
        {
            insert(QPalette::WindowText,        "WindowText");
            insert(QPalette::Button,            "Button");
            insert(QPalette::Light,             "Light");
            insert(QPalette::Midlight,          "Midlight");
            insert(QPalette::Dark,              "Dark");
            insert(QPalette::Mid,               "Mid");
            insert(QPalette::Text,              "Text");
            insert(QPalette::BrightText,        "BrightText");
            insert(QPalette::ButtonText,        "ButtonText");
            insert(QPalette::Base,              "Base");
            insert(QPalette::Window,            "Window");
            insert(QPalette::Shadow,            "Shadow");
            insert(QPalette::Highlight,         "Highlight");
            insert(QPalette::HighlightedText,   "HighlightedText");
            insert(QPalette::Link,              "Link");
            insert(QPalette::LinkVisited,       "LinkVisited");
            insert(QPalette::AlternateBase,     "AlternateBase");
            insert(QPalette::NoRole,            "NoRole");
            insert(QPalette::ToolTipBase,       "ToolTipBase");
            insert(QPalette::ToolTipText,       "ToolTipText");
            insert(QPalette::PlaceholderText,   "PlaceholderText");
        }
    };

    static const ColorGroupToStringMap _ColorGroupToStringMap;
    static const ColorRoleToStringMap _ColorRoleToStringMap;
};

Q_DECLARE_METATYPE(Palette)

#endif // PALETTE_H
