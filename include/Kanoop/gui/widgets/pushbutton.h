#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QPushButton subclass with stylesheet-based color and font customization.
 *
 * PushButton extends QPushButton to provide programmatic control over foreground,
 * background, hover, pressed, and disabled state colors via Qt stylesheets.
 * It also offers convenience methods for font size, weight, and style changes.
 */
class LIBKANOOPGUI_EXPORT PushButton : public QPushButton
{
    Q_OBJECT

    /** @brief The current foreground (text) color of the button. */
    Q_PROPERTY(QColor foregroundColor READ foregroundColor WRITE setForegroundColor) // clazy:exclude=qproperty-without-notify
    /** @brief The current background color of the button. */
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor) // clazy:exclude=qproperty-without-notify

public:
    /** @brief Construct a PushButton with an optional parent widget. */
    explicit PushButton(QWidget *parent = nullptr);
    /** @brief Construct a PushButton with the given text and optional parent widget. */
    explicit PushButton(const QString &text, QWidget *parent = nullptr);
    /** @brief Construct a PushButton with the given icon, text, and optional parent widget. */
    PushButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);

    /** @brief Set the font point size. */
    void setFontPointSize(int size);
    /** @brief Set the font pixel size. */
    void setFontPixelSize(int size);
    /** @brief Set the font bold state. */
    void setBold(bool bold);
    /** @brief Set the font italic state. */
    void setItalic(bool italic);

    /** @brief Get the current foreground color. */
    QColor foregroundColor() const { return _foregroundColor; }
    /** @brief Get the current background color. */
    QColor backgroundColor() const { return _backgroundColor; }

    /** @brief Set the border radius in pixels. */
    void setBorderRadius(int radius);

    /** @brief Set the foreground color when the mouse hovers over the button. */
    void setHoverForegroundColor(const QColor& color);
    /** @brief Set the background color when the mouse hovers over the button. */
    void setHoverBackgroundColor(const QColor& color);
    /** @brief Set the foreground color when the button is pressed. */
    void setPressedForegroundColor(const QColor& color);
    /** @brief Set the background color when the button is pressed. */
    void setPressedBackgroundColor(const QColor& color);
    /** @brief Set the foreground color when the button is disabled. */
    void setDisabledForegroundColor(const QColor& color);
    /** @brief Set the background color when the button is disabled. */
    void setDisabledBackgroundColor(const QColor& color);

public slots:
    /** @brief Set the foreground (text) color and rebuild the stylesheet. */
    void setForegroundColor(const QColor& color);
    /** @brief Set the background color and rebuild the stylesheet. */
    void setBackgroundColor(const QColor& color);
    /** @brief Reset the foreground color to the palette default. */
    void setDefaultForegroundColor();
    /** @brief Reset the background color to the palette default. */
    void setDefaultBackgroundColor();

private:
    void commonInit();
    void makeStyleSheet();

    QColor _foregroundColor;
    QColor _backgroundColor;
    QColor _hoverForegroundColor;    // invalid QColor() = not set
    QColor _hoverBackgroundColor;
    QColor _pressedForegroundColor;
    QColor _pressedBackgroundColor;
    QColor _disabledForegroundColor;
    QColor _disabledBackgroundColor;
    int    _borderRadius = 0;
};

#endif // PUSHBUTTON_H
