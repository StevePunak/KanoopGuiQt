#ifndef MDISUBWINDOW_H
#define MDISUBWINDOW_H
#include <Kanoop/gui/libkanoopgui.h>
#include <QMdiSubWindow>

/**
 * @brief QMdiSubWindow subclass that persists geometry and emits a closing() signal.
 *
 * MdiSubWindow saves and restores its position and size via GuiSettings and emits
 * closing() before the window is destroyed so that parent windows can clean up.
 * An integer type() property allows the application to categorize open sub-windows.
 */
class LIBKANOOPGUI_EXPORT MdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit MdiSubWindow(QWidget* parent = nullptr);

    /**
     * ⚠ MdiWindow::openSubWindow() builds every sub-window's object name as the child window's
     * name plus this suffix, and geometry is keyed on that name. Changing it orphans every
     * stored sub-window position and size.
     */
    static constexpr const char* MdiSubWindowSuffix = "-mdiSub";

    /**
     * @brief Return the application-defined sub-window type integer.
     * @return Sub-window type value
     */
    int type() const { return _type; }

    /**
     * @brief Set the application-defined sub-window type integer.
     *
     * ⚠ One window class must be opened under one type only. MdiWindow::openSubWindow() decides
     * whether to offset a new window by looking for open windows of the same TYPE, while the
     * geometry it restores is keyed on the window's CLASS. Under two types neither window of that
     * class sees the other, and both are placed at the single position stored for the class.
     *
     * ⚠ One type shared by two classes is permitted, and costs the second class its stored
     * position: it is offset from whatever of the shared type is already open instead.
     *
     * ⚠ The type is an integer of the application's choosing and need not resemble the class name,
     * so neither rule is visible at the call site.
     *
     * @param value Sub-window type value
     */
    void setType(int value) { _type = value; }

private:
    /** @brief Persist position on move. */
    virtual void moveEvent(QMoveEvent* event) override;
    /** @brief Persist size on resize. */
    virtual void resizeEvent(QResizeEvent* event) override;
    /** @brief Mark form load complete on first show. */
    virtual void showEvent(QShowEvent *event) override;
    /** @brief Emit closing() before the window closes. */
    virtual void closeEvent(QCloseEvent* event) override;

    /**
     * @brief The name under which geometry shared by this window's kind is keyed.
     * @return The child's kind name plus the sub-window suffix, or empty when there is no child
     */
    QString geometryKindObjectName() const;

    bool _formLoadComplete = false;
    int _type = 0;

signals:
    /** @brief Emitted just before the sub-window closes. */
    void closing();

public slots:
    /**
     * @brief Preferences-change hook.  The default implementation applies the GuiSettings
     *        font size to this widget, so an override must call the base implementation
     *        to keep it.
     */
    virtual void onPreferencesChanged();
};

#endif // MDISUBWINDOW_H
