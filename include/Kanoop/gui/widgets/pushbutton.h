#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QPushButton subclass with font size and color helpers.
 *
 * PushButton provides setFontPointSize(), setFontPixelSize(),
 * setForegroundColor(), and setBackgroundColor() as convenient alternatives
 * to manual stylesheet construction.
 */
class LIBKANOOPGUI_EXPORT PushButton : public QPushButton
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit PushButton(QWidget *parent = nullptr);

    /**
     * @brief Construct with label text.
     * @param text Button label
     * @param parent Optional QWidget parent
     */
    explicit PushButton(const QString &text, QWidget *parent = nullptr);

    /**
     * @brief Construct with an icon and label text.
     * @param icon Button icon
     * @param text Button label
     * @param parent Optional QWidget parent
     */
    PushButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);

    /**
     * @brief Set the font size in points.
     * @param size Point size
     */
    void setFontPointSize(int size);

    /**
     * @brief Set the font size in pixels.
     * @param size Pixel size
     */
    void setFontPixelSize(int size);

    /**
     * @brief Set the foreground (text) color via a stylesheet.
     * @param color New foreground color
     */
    void setForegroundColor(const QColor& color);

    /**
     * @brief Set the background color via a stylesheet.
     * @param color New background color
     */
    void setBackgroundColor(const QColor& color);

private:
    void commonInit();
    void makeStyleSheet();

    QColor _backgroundColor;
    QColor _foregroundColor;
};

#endif // PUSHBUTTON_H
