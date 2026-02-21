#ifndef TABBAR_H
#define TABBAR_H

#include <QTabBar>

/**
 * @brief QTabBar subclass that emits a context-menu signal for individual tab indices.
 *
 * TabBar intercepts right-click events and emits tabCustomContextMenuRequested()
 * with the index of the tab under the cursor, allowing callers to show a
 * per-tab context menu without subclassing further.
 */
class TabBar : public QTabBar
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit TabBar(QWidget *parent = nullptr);

protected:
    /** @brief Intercept right-click presses to emit the context-menu signal. */
    virtual void mousePressEvent(QMouseEvent* event) override;

signals:
    /**
     * @brief Emitted when the user right-clicks a tab.
     * @param index Logical index of the tab that was right-clicked
     */
    void tabCustomContextMenuRequested(int index);
};

#endif // TABBAR_H
