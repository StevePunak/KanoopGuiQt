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

    enum TextFlag
    {
        NoTextFlags     = 0x0000,
        BoldText        = 0x0001,
        StrongText      = 0x0002,
    };
    Q_DECLARE_FLAGS(TextFlags, TextFlag)

    void appendText(const QString& text, const QColor& foregroundColor = QColor(), const QColor& backgroundColor = QColor());
    void appendFormattedText(const QString& text, TextFlags flags, const QColor& foregroundColor = QColor(), const QColor& backgroundColor = QColor());
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PlainTextEdit::TextFlags)

#endif // PLAINTEXTEDIT_H
