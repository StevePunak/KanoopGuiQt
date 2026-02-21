#ifndef MDIWINDOW_H
#define MDIWINDOW_H
#include <Kanoop/gui/mainwindowbase.h>
#include <Kanoop/gui/libkanoopgui.h>

class QMdiSubWindow;
class MdiSubWindow;

/**
 * @brief MainWindowBase subclass that manages an embedded QMdiArea.
 *
 * MdiWindow provides helpers for opening, closing, and finding MdiSubWindow
 * instances by type, and receives a subWindowClosing() callback when any
 * sub-window signals that it is about to close.
 */
class LIBKANOOPGUI_EXPORT MdiWindow : public MainWindowBase
{
    Q_OBJECT
public:
    /**
     * @brief Construct with a log prefix and optional parent.
     * @param logPrefix Category name used for log output
     * @param parent Optional QWidget parent
     */
    MdiWindow(const QString& logPrefix, QWidget* parent = nullptr);

protected:
    /**
     * @brief Return the currently active sub-window's MainWindowBase.
     * @return Pointer to the active sub-window's base, or nullptr
     */
    MainWindowBase* activeSubWindow();

    /**
     * @brief Open a new MdiSubWindow wrapping the given MainWindowBase.
     * @param window Content window to host
     * @param type Application-defined sub-window type integer
     * @return Pointer to the created MdiSubWindow
     */
    virtual MdiSubWindow* openSubWindow(MainWindowBase* window, int type);

    /**
     * @brief Close all open sub-windows of the given type.
     * @param type Application-defined sub-window type integer
     */
    virtual void closeSubWindows(int type);

    /**
     * @brief Return the first open sub-window of the given type.
     * @param type Application-defined sub-window type integer
     * @return Pointer to the first matching MdiSubWindow, or nullptr
     */
    MdiSubWindow* findFirstMdiSubWindow(int type) const;

    /**
     * @brief Return all open sub-windows of the given type.
     * @param type Application-defined sub-window type integer
     * @return List of matching MdiSubWindow pointers
     */
    QList<MdiSubWindow*> findMdiSubWindows(int type) const;

    /**
     * @brief Return the embedded QMdiArea.
     * @return Pointer to the QMdiArea
     */
    QMdiArea* mdiArea();

    /**
     * @brief Called just before a sub-window closes (no-op by default).
     * @param window The sub-window that is closing
     */
    virtual void subWindowClosing(MdiSubWindow* window) { Q_UNUSED(window) }

private:
    QMdiArea* _mdiArea = nullptr;

private slots:
    /** @brief Internal handler forwarding sub-window close events to subWindowClosing(). */
    void onSubWindowClosing();
};

#endif // MDIWINDOW_H
