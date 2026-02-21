#ifndef HTMLUTIL_H
#define HTMLUTIL_H

#include <QColor>
#include <QString>

/**
 * @brief Static helpers for generating common HTML tag strings.
 *
 * Each method returns a QString containing the appropriate HTML tag or
 * attribute, ready to be concatenated into an HTML fragment.
 */
class HtmlUtil
{
public:
    /**
     * @brief Return an opening &lt;p&gt; tag with optional inline color styles.
     * @param color Foreground text color (invalid QColor = omitted)
     * @param backgroundColor Background color (invalid QColor = omitted)
     * @return HTML string for the opening paragraph tag
     */
    static QString startParagraph(const QColor& color = QColor(), const QColor& backgroundColor = QColor());

    /**
     * @brief Return a closing &lt;/p&gt; tag.
     * @return "&lt;/p&gt;"
     */
    static QString endParagraph();

    /**
     * @brief Return an opening &lt;b&gt; tag.
     * @return "&lt;b&gt;"
     */
    static QString startBold();

    /**
     * @brief Return a closing &lt;/b&gt; tag.
     * @return "&lt;/b&gt;"
     */
    static QString endBold();

    /**
     * @brief Return an opening &lt;strong&gt; tag.
     * @return "&lt;strong&gt;"
     */
    static QString startStrong();

    /**
     * @brief Return a closing &lt;/strong&gt; tag.
     * @return "&lt;/strong&gt;"
     */
    static QString endStrong();
};

#endif // HTMLUTIL_H
