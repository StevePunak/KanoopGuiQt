#ifndef FRAME_H
#define FRAME_H

#include <QFrame>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QFrame subclass with logging support and color Q_PROPERTYs.
 *
 * Frame exposes foregroundColor and backgroundColor as Qt properties,
 * allowing them to be set from Qt Designer or stylesheets and causing the
 * widget to repaint with the chosen colors.  The two colors are held
 * independently, so setting one leaves the other in place.
 */
class LIBKANOOPGUI_EXPORT Frame : public QFrame,
                                  public LoggingBaseClass
{
    Q_OBJECT
    /** @brief Foreground (text) color of the frame. */
    Q_PROPERTY(QColor foregroundColor READ foregroundColor WRITE setForegroundColor)    // clazy:exclude=qproperty-without-notify
    /** @brief Background color of the frame. */
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)    // clazy:exclude=qproperty-without-notify
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit Frame(QWidget *parent = nullptr);

    /**
     * @brief Return the current foreground color.
     * @return Foreground (text) color, or an invalid QColor when none has been set
     */
    QColor foregroundColor() const { return _foregroundColor; }

    /**
     * @brief Set the foreground color and repaint.
     * @param color New foreground color
     */
    void setForegroundColor(const QColor& color);

    /**
     * @brief Return the current background color.
     * @return Background color, or an invalid QColor when none has been set
     */
    QColor backgroundColor() const { return _backgroundColor; }

    /**
     * @brief Set the background color and repaint.
     * @param color New background color
     */
    void setBackgroundColor(const QColor& color);

    /** @brief Clear the explicit foreground color so the widget paints from the palette. */
    void setDefaultForegroundColor();

    /** @brief Clear the explicit background color so the widget paints from the palette. */
    void setDefaultBackgroundColor();

private:
    void applyStylesheet();

    QColor _foregroundColor;
    QColor _backgroundColor;
    bool _foregroundExplicitlySet = false;
    bool _backgroundExplicitlySet = false;
};

#endif // FRAME_H
