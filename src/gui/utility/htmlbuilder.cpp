#include "utility/htmlbuilder.h"


HtmlBuilder::HtmlBuilder() :
    _output(&_result)
{
}

void HtmlBuilder::startParagraph(const QColor& color, const QColor& backgroundColor)
{
    _output << HtmlUtil::startParagraph(color, backgroundColor);
}

void HtmlBuilder::endParagraph()
{
    _output << HtmlUtil::endParagraph();
}

void HtmlBuilder::startBold()
{
    _output << HtmlUtil::startBold();
}

void HtmlBuilder::endBold()
{
    _output << HtmlUtil::endBold();
}

void HtmlBuilder::startStrong()
{
    _output << HtmlUtil::startStrong();
}

void HtmlBuilder::endStrong()
{
    _output << HtmlUtil::endStrong();
}

void HtmlBuilder::startTable(int cellPadding, int cellSpacing)
{
    _output << HtmlUtil::startTable(cellPadding, cellSpacing);
}

void HtmlBuilder::endTable()
{
    _output << HtmlUtil::endTable();
}

void HtmlBuilder::startRow()
{
    _output << HtmlUtil::startRow();
}

void HtmlBuilder::endRow()
{
    _output << HtmlUtil::endRow();
}

void HtmlBuilder::startCell(Qt::Alignment alignment)
{
    _output << HtmlUtil::startCell(alignment);
}

void HtmlBuilder::endCell()
{
    _output << HtmlUtil::endCell();
}

void HtmlBuilder::appendCell(const QString& text, Qt::Alignment alignment)
{
    _output << HtmlUtil::startCell(alignment) << HtmlUtil::escape(text) << HtmlUtil::endCell();
}

void HtmlBuilder::appendText(const QString& text)
{
    _output << text;
}
