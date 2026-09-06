#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QLineEdit subclass with foreground and background color helpers.
 *
 * LineEdit provides setForegroundColor() and setBackgroundColor().  Each call replaces
 * the widget's whole stylesheet, so the second of the two setters discards the colour the
 * first one applied.
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

signals:

};

#endif // LINEEDIT_H
