#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <QCheckBox>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QCheckBox subclass with an optional read-only mode.
 *
 * CheckBox exposes a readOnly property.  When set, mouse press events are ignored.
 * Keyboard and mnemonic activation still change the check state.
 */
class LIBKANOOPGUI_EXPORT CheckBox : public QCheckBox
{
    /** @brief Whether the checkbox ignores user mouse presses. */
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)             // clazy:exclude=qproperty-without-notify
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit CheckBox(QWidget *parent = nullptr);

    /**
     * @brief Construct with initial label text.
     * @param text Checkbox label
     * @param parent Optional QWidget parent
     */
    explicit CheckBox(const QString &text, QWidget *parent = nullptr);

    /**
     * @brief Return whether the checkbox is read-only.
     * @return true if mouse presses are ignored
     */
    bool isReadOnly() const { return _readOnly; }

public slots:
    /**
     * @brief Enable or disable the read-only mode.
     * @param value true to prevent user changes
     */
    void setReadOnly(bool value) { _readOnly = value; }

private:
    virtual void mousePressEvent(QMouseEvent* event) override;

private:
    bool _readOnly = false;
};

#endif // CHECKBOX_H
