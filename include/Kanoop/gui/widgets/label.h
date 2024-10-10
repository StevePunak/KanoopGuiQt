#ifndef LABEL_H
#define LABEL_H

#include <QLabel>

class Label : public QLabel
{
    Q_OBJECT
public:
    explicit Label(QWidget *parent = nullptr, Qt::WindowFlags f=Qt::WindowFlags());
    explicit Label(const QString &text, QWidget *parent = nullptr, Qt::WindowFlags f=Qt::WindowFlags());

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

#endif // LABEL_H
