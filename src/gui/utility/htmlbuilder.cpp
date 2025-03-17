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

void HtmlBuilder::appendText(const QString& text)
{
    _output << text;
}
