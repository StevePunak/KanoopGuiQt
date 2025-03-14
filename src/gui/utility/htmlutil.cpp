#include "utility/htmlutil.h"

#include <QColor>
#include <QTextStream>

QString HtmlUtil::startParagraph(const QColor& color, const QColor& backgroundColor)
{
    QString result;
    QTextStream output(&result);
    output << "<p";
    if(color.isValid()) {
        output << " style=\"color: " << color.name(QColor::HexArgb) << ';';
        if(backgroundColor.isValid()) {
            output << "background-color: " << backgroundColor.name(QColor::HexArgb) << ';';
        }
        output << '\"';
    }
    output << '>';
    return result;
}

QString HtmlUtil::endParagraph()
{
    return "</p>";
}
