#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <QCheckBox>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QCheckBox subclass with an optional read-only mode.
 *
 * CheckBox exposes a readOnly property.  When set, mouse and keyboard input no longer
 * change the check state, and the widget stays enabled.  Mnemonic activation and
 * programmatic click() still change it.
 */
class LIBKANOOPGUI_EXPORT CheckBox : public QCheckBox
{
    /** @brief Whether the checkbox ignores user mouse and keyboard input. */
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
     * @return true if mouse and keyboard input are ignored
     */
    bool isReadOnly() const { return _readOnly; }

public slots:
    /**
     * @brief Enable or disable the read-only mode.
     * @param value true to suppress mouse and keyboard input
     */
    void setReadOnly(bool value) { _readOnly = value; }

private:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

private:
    bool _readOnly = false;
};

#endif // CHECKBOX_H
