#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <Kanoop/kanoopcommon.h>
#include <Kanoop/gui/utility/stylesheettypes.h>
#include <Kanoop/gui/libkanoopgui.h>

#include <QWidget>

/**
 * @brief Template helper for building typed Qt stylesheet strings.
 *
 * StyleSheet<T> is parameterized on a QWidget subclass.  It captures the
 * widget's class name at construction and accumulates property/value pairs
 * that are rendered into a complete stylesheet rule by toString().
 *
 * An optional pseudo-state (e.g., PS_Hover) and sub-control selector can
 * narrow the rule to a specific widget state or part.
 *
 * @tparam T QWidget subclass whose class name is used as the selector
 */
template <typename T>
class LIBKANOOPGUI_EXPORT StyleSheet
{
public:
    static_assert(std::is_base_of<QWidget, T>::value, "Templatized class type must be subclass of QWidget");

    /** @brief Construct and capture the widget class name as the CSS selector. */
    StyleSheet()
    {
        T* t = new T();
        _typeName = t->metaObject()->className();
        delete t;
    }

    /**
     * @brief Set a stylesheet property to a string value.
     * @param property Property identifier
     * @param value String value for the property
     */
    void setProperty(StyleSheetProperty property, const QString& value);

    /**
     * @brief Set a stylesheet property to a color value.
     * @param property Property identifier
     * @param value Color to convert to a CSS color string
     */
    void setProperty(StyleSheetProperty property, const QColor& value);

    /**
     * @brief Set a stylesheet property to a pixel integer value.
     * @param property Property identifier
     * @param value Size in pixels
     */
    void setPropertyPixels(StyleSheetProperty property, int value);

    /**
     * @brief Set the pseudo-state selector (e.g., PS_Hover, PS_Checked).
     * @param value Pseudo-state enum value
     */
    void setPseudoState(StyleSheetPseudoState value) { _pseudoState = value; }

    /**
     * @brief Set the sub-control selector string (e.g., "::handle").
     * @param value Sub-control string
     */
    void setSubControl(const QString& value) { _subControl = value; }

    /**
     * @brief Render all accumulated properties into a complete stylesheet rule.
     * @return Stylesheet string suitable for QWidget::setStyleSheet()
     */
    QString toString() const;


    /** @brief CSS selector class name (set from T::metaObject()->className()). */
    QString _typeName;

    /** @brief Accumulated property/value pairs for the stylesheet rule. */
    QMap<StyleSheetProperty, QString> _properties;
    /** @brief Pseudo-state selector applied to the rule (PS_Invalid = none). */
    StyleSheetPseudoState _pseudoState = PS_Invalid;
    /** @brief Sub-control selector string (empty = none). */
    QString _subControl;
};

#endif // STYLESHEET_H
