#include "widgets/plaintextedit.h"

#include <utility/htmlbuilder.h>

void PlainTextEdit::appendText(const QString& text, const QColor& foregroundColor, const QColor& backgroundColor)
{
    appendFormattedText(text, NoTextFlags, foregroundColor, backgroundColor);
}

void PlainTextEdit::appendFormattedText(const QString& text, TextFlags flags, const QColor& foregroundColor, const QColor& backgroundColor)
{
    HtmlBuilder html;
    html.startParagraph(foregroundColor, backgroundColor);
    if(flags & BoldText) {
        html.startBold();
    }
    if(flags & StrongText) {
        html.startStrong();
    }
    html.appendText(text);

    if(flags & StrongText) {
        html.endStrong();
    }
    if(flags & BoldText) {
        html.endBold();
    }

    html.endParagraph();
    QString result = html.toString();
    appendHtml(result);
}
