#ifndef HTMLUTIL_H
#define HTMLUTIL_H

#include <QColor>
#include <QString>

class HtmlUtil
{
public:
    static QString startParagraph(const QColor& color = QColor(), const QColor& backgroundColor = QColor());
    static QString endParagraph();
    static QString startBold();
    static QString endBold();
    static QString startStrong();
    static QString endStrong();
};

#endif // HTMLUTIL_H
