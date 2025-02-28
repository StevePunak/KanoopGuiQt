#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit PlainTextEdit(QWidget *parent = nullptr) :
        QPlainTextEdit(parent) {}
    explicit PlainTextEdit(const QString& text, QWidget *parent = nullptr) :
        QPlainTextEdit(text, parent) {}

    void appendText(const QString& text, const QColor& foregroundColor = QColor(), const QColor& backgroundColor = QColor());
};

#endif // PLAINTEXTEDIT_H
