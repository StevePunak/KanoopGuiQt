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

QString HtmlUtil::startBold()
{
    return "<b>";
}

QString HtmlUtil::endBold()
{
    return "</b>";
}

QString HtmlUtil::startStrong()
{
    return "<strong>";
}

QString HtmlUtil::endStrong()
{
    return "</strong>";
}

QString HtmlUtil::startTable(int cellPadding, int cellSpacing)
{
    QString result;
    QTextStream output(&result);
    output << "<table cellpadding=\"" << cellPadding << "\" cellspacing=\"" << cellSpacing << "\">";
    return result;
}

QString HtmlUtil::endTable()
{
    return "</table>";
}

QString HtmlUtil::startRow()
{
    return "<tr>";
}

QString HtmlUtil::endRow()
{
    return "</tr>";
}

QString HtmlUtil::startCell(Qt::Alignment alignment)
{
    QString align = "left";
    if(alignment.testFlag(Qt::AlignRight)) {
        align = "right";
    }
    else if(alignment.testFlag(Qt::AlignHCenter)) {
        align = "center";
    }

    QString result;
    QTextStream output(&result);
    output << "<td align=\"" << align << "\">";
    return result;
}

QString HtmlUtil::endCell()
{
    return "</td>";
}

QString HtmlUtil::escape(const QString& text)
{
    return text.toHtmlEscaped();
}
