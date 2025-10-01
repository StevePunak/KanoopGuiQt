#ifndef LABEL_H
#define LABEL_H

#include <QLabel>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT Label : public QLabel
{
    Q_OBJECT
public:
    explicit Label(QWidget *parent = nullptr, Qt::WindowFlags f=Qt::WindowFlags());
    explicit Label(const QString &text, QWidget *parent = nullptr, Qt::WindowFlags f=Qt::WindowFlags());

    void setFontPointSize(int size);
    void setFontPixelSize(int size);

    void setForegroundColor(const QColor& color);
    QColor foregroundColor() const { return _foregroundColor; }

    void setBackgroundColor(const QColor& color);
    QColor backgroundColor() const { return _backgroundColor; }

    void setDefaultForegroundColor();
    void setDefaultBackgroundColor();

private:
    void commonInit();
    void makeStyleSheet();

    QColor _backgroundColor;
    QColor _foregroundColor;
};

#endif // LABEL_H
