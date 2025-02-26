/******************************************************************************************
**
** htmlbuilder.h
**
** This is the very early start on an HTML builder class.
**
** Author:  Stephen Punak
** Created: Sun Feb 22 2025
**
******************************************************************************************/
#ifndef HTMLBUILDER_H
#define HTMLBUILDER_H
#include <Kanoop/gui/utility/htmlutil.h>

#include <QTextStream>

class HtmlBuilder
{
public:
    HtmlBuilder();

    void startParagraph(const QColor& color = QColor(), const QColor& backgroundColor = QColor());
    void endParagraph();
    void appendText(const QString& text);

    QString toString() const { return _result; }

private:
    QString _result;
    QTextStream _output;
};

#endif // HTMLBUILDER_H
