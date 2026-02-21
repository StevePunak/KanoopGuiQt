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
     * @brief Return the application-defined sub-window type integer.
     * @return Sub-window type value
     */
    int type() const { return _type; }

    /**
     * @brief Set the application-defined sub-window type integer.
     * @param value Sub-window type value
     */
    void setType(int value) { _type = value; }

private:
    /** @brief Persist position on move. */
    virtual void moveEvent(QMoveEvent* event) override;
    /** @brief Persist size on resize. */
    virtual void resizeEvent(QResizeEvent* event) override;
    /** @brief Restore geometry and mark form load complete on first show. */
    virtual void showEvent(QShowEvent *event) override;
    /** @brief Emit closing() before the window closes. */
    virtual void closeEvent(QCloseEvent* event) override;

    bool _formLoadComplete = false;
    int _type = 0;

signals:
    /** @brief Emitted just before the sub-window closes. */
    void closing();

public slots:
    /** @brief Called when application preferences change; override to react. */
    virtual void onPreferencesChanged();
};

#endif // MDISUBWINDOW_H
