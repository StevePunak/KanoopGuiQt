#ifndef PALETTE_H
#define PALETTE_H
#include <QPalette>
#include <QVariant>
#include <Kanoop/gui/libkanoopgui.h>
#include <Kanoop/kanoopcommon.h>

/**
 * @brief QPalette subclass adding named presets, string lookups, and QVariant serialization.
 *
 * Palette forwards all QPalette constructors and adds:
 * - fusionLight() / fusionDark() factory methods for standard Fusion-style palettes
 * - toVariant() / fromVariant() for QVariant round-trips
 * - String lookup helpers for ColorRole and ColorGroup enum values
 * - debugDumpPalette() for diagnostic output
 */
class LIBKANOOPGUI_EXPORT Palette : public QPalette
{
public:
    /** @brief Default constructor. */
    Palette() :
        QPalette() {}
    /**
     * @brief Construct from a button colour.
     * @param button Button role colour
     */
    Palette(const QColor &button) :
        QPalette(button) {}
    /**
     * @brief Construct from a global colour for the button role.
     * @param button Global colour constant
     */
    Palette(Qt::GlobalColor button) :
        QPalette(button) {}
    /**
     * @brief Construct from button and window colours.
     * @param button Button role colour
     * @param window Window role colour
     */
    Palette(const QColor &button, const QColor &window) :
        QPalette(button, window) {}
    /**
     * @brief Construct from individual brush values for each role.
     * @param windowText WindowText brush
     * @param button Button brush
     * @param light Light brush
     * @param dark Dark brush
     * @param mid Mid brush
     * @param text Text brush
     * @param bright_text BrightText brush
     * @param base Base brush
     * @param window Window brush
     */
    Palette(const QBrush &windowText, const QBrush &button, const QBrush &light,
             const QBrush &dark, const QBrush &mid, const QBrush &text,
             const QBrush &bright_text, const QBrush &base, const QBrush &window) :
        QPalette(windowText, button, light, dark, mid, text, bright_text, base, window) {}
    /**
     * @brief Construct from individual colour values for each role.
     * @param windowText WindowText colour
     * @param window Window colour
     * @param light Light colour
     * @param dark Dark colour
     * @param mid Mid colour
     * @param text Text colour
     * @param base Base colour
     */
    Palette(const QColor &windowText, const QColor &window, const QColor &light,
             const QColor &dark, const QColor &mid, const QColor &text, const QColor &base) :
        QPalette(windowText, window, light, dark, mid, text, base) {}
    /**
     * @brief Copy-construct from a QPalette.
     * @param palette Source palette
     */
    Palette(const QPalette &palette) :
        QPalette(palette) {}

    /**
     * @brief Wrap this palette in a QVariant.
     * @return QVariant containing a copy of this Palette
     */
    QVariant toVariant() const { return QVariant::fromValue<Palette>(*this); }

    /**
     * @brief Unwrap a Palette from a QVariant.
     * @param value Variant previously created by toVariant()
     * @return Extracted Palette
     */
    static Palette fromVariant(const QVariant& value) { return value.value<Palette>(); }

    /** @brief A list of Palette objects. */
    class List : public QList<Palette> {};

    /**
     * @brief Return a light Fusion-style palette.
     * @return Fusion light palette
     */
    static Palette fusionLight();

    /**
     * @brief Return a dark Fusion-style palette.
     * @return Fusion dark palette
     */
    static Palette fusionDark();

    /**
     * @brief Print all colour roles and their values to debug output.
     * @param palette Palette to dump
     */
    static void debugDumpPalette(const Palette& palette);

    /**
     * @brief Return the string name for a colour role.
     * @param role QPalette colour role
     * @return String name, or empty string if unknown
     */
    static QString getColorRoleString(ColorRole role) { return _ColorRoleToStringMap.getString(role); }

    /**
     * @brief Return all registered colour role name strings.
     * @return List of colour role names
     */
    static QStringList getColorRoleStrings() { return _ColorRoleToStringMap.values(); }

    /**
     * @brief Look up a colour role by its string name.
     * @param value Colour role name string
     * @return Matching QPalette::ColorRole, or NoRole if not found
     */
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
