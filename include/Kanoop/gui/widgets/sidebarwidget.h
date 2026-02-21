#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QListView>
#include <Kanoop/entitymetadata.h>
#include <Kanoop/gui/libkanoopgui.h>

class QStandardItemModel;
class SidebarPaintDelegate;

/**
 * @brief Icon-and-text sidebar navigation widget backed by QStandardItemModel.
 *
 * SidebarWidget displays a vertical list of named, optionally-iconed navigation
 * entries.  Each entry is keyed by an entity metadata type integer.  Clicking an
 * entry emits itemClicked() with the entity type.  Spacing, icon size, and content
 * margins are all adjustable.
 */
class LIBKANOOPGUI_EXPORT SidebarWidget : public QListView
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit SidebarWidget(QWidget *parent = nullptr);

    /**
     * @brief Add an entry to the sidebar.
     * @param entityMetadataType Integer key for the entry
     * @param text Display text
     * @param imageResourceId Optional Qt resource ID for the entry icon (0 = none)
     */
    void addItem(int entityMetadataType, const QString& text, int imageResourceId = 0);

    /**
     * @brief Remove the entry with the given entity type.
     * @param entityMetadataType Entity type key of the entry to remove
     */
    void removeItem(int entityMetadataType);

    /**
     * @brief Return whether an entry with the given entity type exists.
     * @param entityMetadataType Entity type key to check
     * @return true if the entry is present
     */
    bool containsItem(int entityMetadataType) const;

    /** @brief Remove all entries from the sidebar. */
    void clear();

    /**
     * @brief Programmatically select the entry with the given entity type.
     * @param entityMetadataType Entity type key to select
     */
    void selectItem(int entityMetadataType);

    /**
     * @brief Return the content margins used by the paint delegate.
     * @return Content margins
     */
    QMargins contentsMargins() const;

    /**
     * @brief Set the content margins used by the paint delegate.
     * @param value New content margins
     */
    void setContentsMargins(const QMargins& value);

    /**
     * @brief Return the horizontal spacing between the icon and text.
     * @return Horizontal spacing in pixels
     */
    int horizontalSpacing() const;

    /**
     * @brief Set the horizontal spacing between the icon and text.
     * @param value Horizontal spacing in pixels
     */
    void setHorizontalSpacing(int value);

    /**
     * @brief Return the vertical spacing between entries.
     * @return Vertical spacing in pixels
     */
    int verticalSpacing() const;

    /**
     * @brief Set the vertical spacing between entries.
     * @param value Vertical spacing in pixels
     */
    void setVerticalSpacing(int value);

    /**
     * @brief Return the icon size used when rendering entries.
     * @return Icon size
     */
    QSize iconSize() const;

    /**
     * @brief Set the icon size used when rendering entries.
     * @param value New icon size
     */
    void setIconSize(const QSize& value);

private:
    QStandardItemModel* sourceModel() const;

    SidebarPaintDelegate* _delegate;

    static const QSize DefaultIconSize;
    static const QMargins DefaultContentsMargins;
    static const int DefaultVerticalSpacing;
    static const int DefaultHorizontalSpacing;

signals:
    /**
     * @brief Emitted when the user clicks a sidebar entry.
     * @param entityType Entity type key of the clicked entry
     */
    void itemClicked(int entityType);

private slots:
    void onItemClicked(const QModelIndex& index);
};

#endif // SIDEBARWIDGET_H
