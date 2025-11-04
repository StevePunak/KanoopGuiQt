#ifndef LABEL_H
#define LABEL_H

#include <QLabel>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT Label : public QLabel
{
    Q_OBJECT

    Q_PROPERTY(QColor foregroundColor READ foregroundColor WRITE setForegroundColor)             // clazy:exclude=qproperty-without-notify
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)            // clazy:exclude=qproperty-without-notify
public:
    explicit Label(QWidget *parent = nullptr, Qt::WindowFlags f=Qt::WindowFlags());
    explicit Label(const QString &text, QWidget *parent = nullptr, Qt::WindowFlags f=Qt::WindowFlags());

    void setFontPointSize(int size);
    void setFontPixelSize(int size);

    QColor foregroundColor() const { return _foregroundColor; }
    QColor backgroundColor() const { return _backgroundColor; }

public slots:
    void setForegroundColor(const QColor& color);
    void setBackgroundColor(const QColor& color);

public:
    void setDefaultForegroundColor();
    void setDefaultBackgroundColor();

private:
    void commonInit();
    void makeStyleSheet();

    QColor _backgroundColor;
    QColor _foregroundColor;
};

#endif // LABEL_H
