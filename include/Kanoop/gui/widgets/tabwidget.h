#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QTabWidget subclass that emits a per-tab context-menu signal.
 *
 * TabWidget connects the tab bar's customContextMenuRequested signal to an
 * internal handler that maps the click position to a tab index and re-emits
 * tabCustomContextMenuRequested() with that index.
 */
class LIBKANOOPGUI_EXPORT TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit TabWidget(QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when the user right-clicks on a tab.
     * @param index Zero-based index of the tab that was right-clicked
     */
    void tabCustomContextMenuRequested(int index);
};

#endif // TABWIDGET_H
