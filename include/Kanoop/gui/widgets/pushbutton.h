#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QWidget *parent = nullptr);
    explicit PushButton(const QString &text, QWidget *parent = nullptr);
    PushButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);

    void setFontPointSize(int size);
    void setFontPixelSize(int size);
    void setForegroundColor(const QColor& color);
    void setBackgroundColor(const QColor& color);

private:
    void commonInit();
    void makeStyleSheet();

    QColor _backgroundColor;
    QColor _foregroundColor;
};

#endif // PUSHBUTTON_H
