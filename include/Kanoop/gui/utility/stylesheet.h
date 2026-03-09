#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <Kanoop/kanoopcommon.h>
#include <Kanoop/gui/utility/stylesheettypes.h>
#include <Kanoop/gui/libkanoopgui.h>

#include <QColor>
#include <QGradient>
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
     * @brief Set the background to a gradient string (qradialgradient / qlineargradient).
     * @param gradient Full CSS gradient value string
     */
    void setGradient(const QString& gradient) { setProperty(SP_Background, gradient); }

    /**
     * @brief Set the background to a radial gradient specified by its geometric parameters.
     *
     * Generates a CSS qradialgradient() value from the given parameters and passes it to
     * setGradient().  Colors are converted to rgba() notation so alpha is always preserved.
     *
     * @param cx      Centre x (0–1)
     * @param cy      Centre y (0–1)
     * @param radius  Radius  (0–1)
     * @param fx      Focal-point x (0–1)
     * @param fy      Focal-point y (0–1)
     * @param stops   Gradient stops (position in 0–1 and color, including alpha)
     */
    void setRadialGradient(double cx, double cy, double radius, double fx, double fy,
                           const QGradientStops& stops)
    {
        QString s = QString("qradialgradient(cx:%1, cy:%2, radius:%3, fx:%4, fy:%5")
            .arg(cx).arg(cy).arg(radius).arg(fx).arg(fy);
        for(const QGradientStop& stop : stops) {
            const QColor& c = stop.second;
            s += QString(", stop:%1 rgba(%2,%3,%4,%5)")
                .arg(stop.first).arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
        }
        s += QLatin1Char(')');
        setGradient(s);
    }

    /**
     * @brief Set all four border properties at once using the dome highlight/shadow convention.
     *
     * Top and left edges receive @p topLeft (highlight); bottom and right receive
     * @p bottomRight (shadow). This matches the standard glassy-dome button style
     * used throughout the UI.
     *
     * @param topLeft     CSS border value for the top and left edges
     * @param bottomRight CSS border value for the bottom and right edges
     */
    void setBorder(const QString& topLeft, const QString& bottomRight)
    {
        setProperty(SP_BorderTop,    topLeft);
        setProperty(SP_BorderLeft,   topLeft);
        setProperty(SP_BorderBottom, bottomRight);
        setProperty(SP_BorderRight,  bottomRight);
    }

    /**
     * @brief Set all four border properties using a pixel width and two colors.
     *
     * Generates "Npx solid rgba(r,g,b,a)" strings for top/left and bottom/right edges,
     * then delegates to the string overload.
     *
     * @param widthPx     Border thickness in pixels
     * @param topLeft     Color for top and left edges (highlight)
     * @param bottomRight Color for bottom and right edges (shadow)
     */
    void setBorder(int widthPx, const QColor& topLeft, const QColor& bottomRight)
    {
        auto make = [widthPx](const QColor& c) {
            return QString("%1px solid rgba(%2,%3,%4,%5)")
                .arg(widthPx).arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
        };
        setBorder(make(topLeft), make(bottomRight));
    }

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
