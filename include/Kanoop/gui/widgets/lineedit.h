#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QLineEdit subclass with foreground and background color helpers.
 *
 * LineEdit provides setForegroundColor() and setBackgroundColor().  The two colors are
 * held independently, so setting one leaves the other in place.  setDefaultForegroundColor()
 * and setDefaultBackgroundColor() drop a color again so the widget paints from the palette.
 */
class LIBKANOOPGUI_EXPORT LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit LineEdit(QWidget *parent = nullptr);

    /**
     * @brief Construct with initial text.
     * @param text Initial text content
     * @param parent Optional QWidget parent
     */
    explicit LineEdit(const QString& text, QWidget *parent = nullptr);

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

    /** @brief Clear the explicit foreground color so the widget paints from the palette. */
    void setDefaultForegroundColor();

    /** @brief Clear the explicit background color so the widget paints from the palette. */
    void setDefaultBackgroundColor();

signals:

private:
    void applyStylesheet();

    QColor _foregroundColor;
    QColor _backgroundColor;
    bool _foregroundExplicitlySet = false;
    bool _backgroundExplicitlySet = false;
};

#endif // LINEEDIT_H
