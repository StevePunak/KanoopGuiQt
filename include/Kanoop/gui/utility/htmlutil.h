#ifndef HTMLUTIL_H
#define HTMLUTIL_H

#include <Kanoop/gui/libkanoopgui.h>
#include <QColor>
#include <QString>

/**
 * @brief Static helpers for generating common HTML tag strings.
 *
 * Each method returns a QString containing the appropriate HTML tag or
 * attribute, ready to be concatenated into an HTML fragment.
 */
class LIBKANOOPGUI_EXPORT HtmlUtil
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

    /**
     * @brief Return an opening &lt;table&gt; tag with the given cell padding/spacing.
     * @param cellPadding Pixels of padding inside each cell
     * @param cellSpacing Pixels of spacing between cells
     * @return HTML string for the opening table tag
     */
    static QString startTable(int cellPadding = 2, int cellSpacing = 0);

    /**
     * @brief Return a closing &lt;/table&gt; tag.
     * @return "&lt;/table&gt;"
     */
    static QString endTable();

    /**
     * @brief Return an opening &lt;tr&gt; tag.
     * @return "&lt;tr&gt;"
     */
    static QString startRow();

    /**
     * @brief Return a closing &lt;/tr&gt; tag.
     * @return "&lt;/tr&gt;"
     */
    static QString endRow();

    /**
     * @brief Return an opening &lt;td&gt; tag with horizontal alignment.
     * @param alignment Horizontal alignment (AlignLeft, AlignRight, AlignHCenter)
     * @return HTML string for the opening cell tag
     */
    static QString startCell(Qt::Alignment alignment = Qt::AlignLeft);

    /**
     * @brief Return a closing &lt;/td&gt; tag.
     * @return "&lt;/td&gt;"
     */
    static QString endCell();

    /**
     * @brief HTML-escape arbitrary text so it can be safely embedded in a fragment.
     * @param text Text to escape
     * @return Escaped text
     */
    static QString escape(const QString& text);
};

#endif // HTMLUTIL_H
