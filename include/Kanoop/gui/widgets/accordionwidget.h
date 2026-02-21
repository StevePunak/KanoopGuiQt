#ifndef ACCORDIONWIDGET_H
#define ACCORDIONWIDGET_H

#include <QPushButton>
#include <QWidget>
#include <Kanoop/gui/libkanoopgui.h>

class AccordionItem;

/**
 * @brief Vertically stacked collapsible-panel (accordion) widget.
 *
 * AccordionWidget manages a list of AccordionItem panels, each with a title
 * button and a content widget.  Items can be added, removed, expanded, and
 * collapsed individually or all at once.
 */
class LIBKANOOPGUI_EXPORT AccordionWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent widget.
     * @param parent Optional QWidget parent
     */
    explicit AccordionWidget(QWidget *parent = nullptr);

    /**
     * @brief Append a new panel at the end.
     * @param title Title shown on the panel's header button
     * @param content Widget displayed when the panel is expanded
     */
    void addItem(const QString& title, QWidget* content);

    /**
     * @brief Insert a new panel at a specific position.
     * @param index Zero-based position to insert at
     * @param title Title shown on the panel's header button
     * @param content Widget displayed when the panel is expanded
     */
    void insertItem(int index, const QString& title, QWidget* content);

    /**
     * @brief Remove the panel at the given index.
     * @param index Zero-based index of the panel to remove
     */
    void removeItem(int index);

    /**
     * @brief Show or hide the panel at the given index.
     * @param index Zero-based index of the panel
     * @param visible true to show, false to hide
     */
    void setItemVisible(int index, bool visible);

    /** @brief Remove all panels from the widget. */
    void clear();

    /** @brief Expand all panels. */
    void expandAll();

    /** @brief Collapse all panels. */
    void collapseAll();

    /**
     * @brief Expand or collapse the panel containing the given content widget.
     * @param widget Content widget identifying the panel
     * @param expanded true to expand, false to collapse
     */
    void setExpanded(QWidget* widget, bool expanded);

    /**
     * @brief Return whether the panel containing the given content widget is expanded.
     * @param widget Content widget identifying the panel
     * @return true if the panel is expanded
     */
    bool isExpanded(QWidget* widget) const;

    /**
     * @brief Return whether all panels are currently expanded.
     * @return true if every panel is expanded
     */
    bool areAllExpanded() const;

    /**
     * @brief Return whether all panels are currently collapsed.
     * @return true if every panel is collapsed
     */
    bool areAllCollapsed() const;

    /**
     * @brief Change the title of the panel containing the given content widget.
     * @param widget Content widget identifying the panel
     * @param title New title text
     */
    void setTitle(QWidget* widget, const QString& title);

    /**
     * @brief Return the index of the panel containing the given content widget.
     * @param widget Content widget to look up
     * @return Zero-based index, or -1 if not found
     */
    int indexOf(QWidget* widget) const;

    /**
     * @brief Return the number of panels.
     * @return Panel count
     */
    int count() const;

signals:
    /** @brief Emitted when a panel is added or removed. */
    void itemCountModified();

private:
    AccordionItem* findItemForWidget(QWidget* widget) const;

public slots:
    /** @brief Called when application preferences change; reapplies styling. */
    virtual void onPreferencesChanged();
};

#endif // ACCORDIONWIDGET_H
