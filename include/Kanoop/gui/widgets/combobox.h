#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QComboBox subclass with bold font helpers and keyboard-commit signals.
 *
 * ComboBox emits accept() when the user presses Enter/Return, and lostFocus()
 * when the widget loses keyboard focus.  Individual rows or the entire widget
 * can have their font weight changed to bold.
 */
class LIBKANOOPGUI_EXPORT ComboBox : public QComboBox
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit ComboBox(QWidget *parent = nullptr);

    /**
     * @brief Set the font weight of all items in the combo box.
     * @param bold true for bold, false for normal weight
     */
    void setBold(bool bold);

    /**
     * @brief Set the font weight of a specific row.
     * @param row Zero-based row index
     * @param bold true for bold, false for normal weight
     */
    void setRowBold(int row, bool bold);

private:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;

signals:
    /** @brief Emitted when the user presses Enter/Return to accept the current value. */
    void accept();
    /** @brief Emitted when the widget loses keyboard focus. */
    void lostFocus();
};

#endif // COMBOBOX_H
