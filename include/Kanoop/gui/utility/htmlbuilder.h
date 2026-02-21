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

/**
 * @brief Streaming HTML fragment builder.
 *
 * HtmlBuilder accumulates HTML into an internal string via a QTextStream.
 * Call the open/close methods in order, then retrieve the result with toString().
 */
class HtmlBuilder
{
public:
    /** @brief Construct an empty builder. */
    HtmlBuilder();

    /**
     * @brief Append an opening &lt;p&gt; tag with optional foreground and background colors.
     * @param color Foreground text color (invalid QColor = none)
     * @param backgroundColor Background color (invalid QColor = none)
     */
    void startParagraph(const QColor& color = QColor(), const QColor& backgroundColor = QColor());

    /** @brief Append a closing &lt;/p&gt; tag. */
    void endParagraph();

    /** @brief Append an opening &lt;b&gt; tag. */
    void startBold();
    /** @brief Append a closing &lt;/b&gt; tag. */
    void endBold();

    /** @brief Append an opening &lt;strong&gt; tag. */
    void startStrong();
    /** @brief Append a closing &lt;/strong&gt; tag. */
    void endStrong();

    /**
     * @brief Append plain text to the output.
     * @param text Text to append (not HTML-escaped)
     */
    void appendText(const QString& text);

    /**
     * @brief Return the accumulated HTML string.
     * @return HTML fragment built so far
     */
    QString toString() const { return _result; }

private:
    QString _result;
    QTextStream _output;
};

#endif // HTMLBUILDER_H
