#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QPlainTextEdit subclass with HTML-formatted and color-styled append helpers.
 *
 * PlainTextEdit adds appendText() for appending text with optional per-call
 * foreground and background colors, and appendFormattedText() which additionally
 * accepts TextFlags for bold/strong formatting.  The text is inserted as HTML and
 * is not escaped.
 */
class LIBKANOOPGUI_EXPORT PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit PlainTextEdit(QWidget *parent = nullptr) :
        QPlainTextEdit(parent) {}

    /**
     * @brief Construct with initial text content.
     * @param text Initial plain text
     * @param parent Optional QWidget parent
     */
    explicit PlainTextEdit(const QString& text, QWidget *parent = nullptr) :
        QPlainTextEdit(text, parent) {}

    /**
     * @brief Flags controlling text formatting for appendFormattedText().
     */
    enum TextFlag
    {
        NoTextFlags     = 0x0000,   ///< No formatting
        BoldText        = 0x0001,   ///< Render text in bold
        StrongText      = 0x0002,   ///< Render text wrapped in &lt;strong&gt;
    };
    Q_DECLARE_FLAGS(TextFlags, TextFlag)

    /**
     * @brief Append a line of text with optional per-call colors.
     * @param text Text to append (not HTML-escaped)
     * @param foregroundColor Text color; an invalid QColor emits no colour styling at all
     * @param backgroundColor Background color; emitted only when @p foregroundColor is also valid
     */
    void appendText(const QString& text, const QColor& foregroundColor = QColor(), const QColor& backgroundColor = QColor());

    /**
     * @brief Append a formatted line of text with flags and optional colors.
     * @param text Text to append
     * @param flags Combination of TextFlag values
     * @param foregroundColor Text color; an invalid QColor emits no colour styling at all
     * @param backgroundColor Background color; emitted only when @p foregroundColor is also valid
     */
    void appendFormattedText(const QString& text, TextFlags flags, const QColor& foregroundColor = QColor(), const QColor& backgroundColor = QColor());
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PlainTextEdit::TextFlags)

#endif // PLAINTEXTEDIT_H
