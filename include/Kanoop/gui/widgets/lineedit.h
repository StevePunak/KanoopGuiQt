#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEdit(QWidget *parent = nullptr);
    explicit LineEdit(const QString& text, QWidget *parent = nullptr);

    void setForegroundColor(const QColor& color);
    void setBackgroundColor(const QColor& color);

signals:

};

#endif // LINEEDIT_H
