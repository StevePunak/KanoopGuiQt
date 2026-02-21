#ifndef LABEL_H
#define LABEL_H

#include <QLabel>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QLabel subclass with font size and color Q_PROPERTYs.
 *
 * Label exposes foregroundColor and backgroundColor as Qt properties,
 * allowing them to be set from Qt Designer or stylesheets.  It also
 * provides setFontPointSize() and setFontPixelSize() for quick font-size
 * changes without manual QFont manipulation.
 */
class LIBKANOOPGUI_EXPORT Label : public QLabel
{
    Q_OBJECT

    /** @brief Foreground (text) color of the label. */
    Q_PROPERTY(QColor foregroundColor READ foregroundColor WRITE setForegroundColor)             // clazy:exclude=qproperty-without-notify
    /** @brief Background color of the label. */
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)            // clazy:exclude=qproperty-without-notify
public:
    /**
     * @brief Construct with optional parent and window flags.
     * @param parent Optional QWidget parent
     * @param f Optional window flags
     */
    explicit Label(QWidget *parent = nullptr, Qt::WindowFlags f=Qt::WindowFlags());

    /**
     * @brief Construct with initial text, optional parent and window flags.
     * @param text Initial label text
     * @param parent Optional QWidget parent
     * @param f Optional window flags
     */
    explicit Label(const QString &text, QWidget *parent = nullptr, Qt::WindowFlags f=Qt::WindowFlags());

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
     * @brief Return the current foreground color.
     * @return Foreground (text) color
     */
    QColor foregroundColor() const { return _foregroundColor; }

    /**
     * @brief Return the current background color.
     * @return Background color
     */
    QColor backgroundColor() const { return _backgroundColor; }

public slots:
    /**
     * @brief Set the foreground (text) color and repaint.
     * @param color New foreground color
     */
    void setForegroundColor(const QColor& color);

    /**
     * @brief Set the background color and repaint.
     * @param color New background color
     */
    void setBackgroundColor(const QColor& color);

public:
    /** @brief Reset the foreground color to the palette default. */
    void setDefaultForegroundColor();
    /** @brief Reset the background color to the palette default. */
    void setDefaultBackgroundColor();

protected:
    /** @brief Rebuild and apply the stylesheet from current color properties. */
    virtual void applyStylesheet();

private:
    void commonInit();

    QColor _backgroundColor;
    QColor _foregroundColor;
};

#endif // LABEL_H
