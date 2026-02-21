#ifndef MDIAREA_H
#define MDIAREA_H
#include <QMdiArea>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QMdiArea subclass that adds keyboard-driven sub-window cycling.
 *
 * MdiArea intercepts keyboard events to cycle through sub-windows and emits
 * signals at the start and end of each paging operation so the application can
 * update its UI accordingly.
 */
class LIBKANOOPGUI_EXPORT MdiArea : public QMdiArea
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit MdiArea(QWidget* parent = nullptr);

signals:
    /**
     * @brief Emitted when a sub-window paging cycle begins for the given window.
     * @param subWindow The sub-window being paged to
     */
    void subWindowPaging(QMdiSubWindow* subWindow);

    /** @brief Emitted when the paging cycle has completed. */
    void pagingComplete();

protected:
    /** @brief Intercept keyboard events to drive sub-window cycling. */
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    /**
     * @brief Return the next sub-window in the cycle order.
     * @param forward true to cycle forward, false to cycle backward
     * @return Pointer to the next sub-window
     */
    QMdiSubWindow *getNextSubWindowInCycle(bool forward = true);

    int _pagingIndex = -1;
};

#endif // MDIAREA_H
