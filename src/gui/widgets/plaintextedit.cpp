#include "widgets/plaintextedit.h"

#include <utility/htmlbuilder.h>

void PlainTextEdit::appendText(const QString& text, const QColor& foregroundColor, const QColor& backgroundColor)
{
    HtmlBuilder html;
    html.startParagraph(foregroundColor, backgroundColor);
    html.appendText(text);
    html.endParagraph();
    QString result = html.toString();
    appendHtml(result);
}
