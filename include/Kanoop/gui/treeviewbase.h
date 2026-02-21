/******************************************************************************************
**
** treeviewbase.h
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Sun Oct 22 17:41:53 2023
**
******************************************************************************************/
#ifndef TREEVIEWBASE_H
#define TREEVIEWBASE_H
#include <Kanoop/entitymetadata.h>

#include <QTreeView>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class QSortFilterProxyModel;
class QStyledItemDelegate;
class AbstractItemModel;

/**
 * @brief QTreeView subclass integrating AbstractItemModel with rich navigation helpers.
 *
 * TreeViewBase wraps its source model in a QSortFilterProxyModel and provides helpers
 * for UUID-based navigation, text-based forward/backward search, recursive collapse,
 * column visibility management, header state persistence, and delegate assignment.
 */
class LIBKANOOPGUI_EXPORT TreeViewBase : public QTreeView,
                                         public LoggingBaseClass
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent widget.
     * @param parent Optional QWidget parent
     */
    TreeViewBase(QWidget* parent = nullptr);

    /**
     * @brief Return the entity type of the item at a view position.
     * @param pos View-local position
     * @return Entity type integer, or -1 if no item at that position
     */
    int entityTypeAtPos(const QPoint& pos);

    /**
     * @brief Return the EntityMetadata of the currently selected item.
     * @return EntityMetadata for the current item
     */
    virtual EntityMetadata currentMetadata() const;

    /**
     * @brief Return the EntityMetadata of the item at a view position.
     * @param pos View-local position
     * @return EntityMetadata at that position
     */
    virtual EntityMetadata metadataAtPos(const QPoint& pos) const;

    /**
     * @brief Return the first index whose item UUID matches.
     * @param uuid UUID to search for
     * @return First matching index, or invalid index if not found
     */
    virtual QModelIndex firstIndexOfEntityUuid(const QUuid& uuid) const;

    /**
     * @brief Return all indexes whose item UUID matches.
     * @param uuid UUID to search for
     * @return List of matching indexes
     */
    virtual QModelIndexList indexesOfUuid(const QUuid& uuid) const;

    /**
     * @brief Select the item with the given UUID and optionally scroll to it.
     * @param uuid UUID of the item to select
     * @param scrollHint How to scroll to make the item visible
     */
    virtual void setCurrentUuid(const QUuid& uuid, ScrollHint scrollHint = EnsureVisible);

    /**
     * @brief Set the current index and optionally scroll to it.
     * @param index Index to make current
     * @param scrollHint How to scroll to make the item visible
     */
    virtual void setCurrentIndex(const QModelIndex& index, ScrollHint scrollHint = EnsureVisible);

    /**
     * @brief Find the next index whose display text matches, searching forward.
     * @param text Text to search for
     * @param fromIndex Starting index for the search
     * @return Next matching index, or invalid index if not found
     */
    virtual QModelIndex findNextMatch(const QString& text, const QModelIndex& fromIndex) const;

    /**
     * @brief Find the previous index whose display text matches, searching backward.
     * @param text Text to search for
     * @param fromIndex Starting index for the backward search
     * @return Previous matching index, or invalid index if not found
     */
    virtual QModelIndex findPreviousMatch(const QString& text, const QModelIndex& fromIndex) const;

    /**
     * @brief Return the deepest last child of the given index.
     * @param from Starting index
     * @return Last descendant index
     */
    virtual QModelIndex finalChildIndex(const QModelIndex& from) const;

    /**
     * @brief Return the next index in display order after from.
     * @param from Reference index
     * @return Next index, or invalid if at the end
     */
    virtual QModelIndex nextIndex(const QModelIndex& from) const;

    /**
     * @brief Return the previous index in display order before from.
     * @param from Reference index
     * @return Previous index, or invalid if at the beginning
     */
    virtual QModelIndex previousIndex(const QModelIndex& from) const;

    /**
     * @brief Search backward from start for indexes matching a role value.
     * @param start Index to begin searching from
     * @param role Model role to compare
     * @param value Value to match
     * @param hits Maximum number of matches to return
     * @param flags Match flags
     * @return List of matching indexes in reverse order
     */
    virtual QModelIndexList matchBackwards(const QModelIndex &start, int role,
                                           const QVariant &value, int hits = 1,
                                           Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const;

    /**
     * @brief Return the current source model index (mapped through the proxy).
     * @return Source model index corresponding to the current view index
     */
    QModelIndex currentSourceIndex() const;

    /**
     * @brief Save the header and expansion state to a byte array.
     * @return Serialized state
     */
    QByteArray saveState() const;

    /**
     * @brief Restore header and expansion state from a byte array.
     * @param state Previously saved state
     */
    void restoreState(const QByteArray& state);

    /** @brief Restore horizontal header state (column widths/order) from settings. */
    void restoreHeaderStates();

    /**
     * @brief Set the model, wrapping it in an internal sort/filter proxy.
     * @param model Model to display (should derive from AbstractItemModel)
     */
    virtual void setModel(QAbstractItemModel* model) override;

    /**
     * @brief Set the selection model.
     * @param selectionModel Selection model to use
     */
    virtual void setSelectionModel(QItemSelectionModel* selectionModel) override;

    /**
     * @brief Return the underlying AbstractItemModel (without the proxy).
     * @return Pointer to the source model
     */
    AbstractItemModel* sourceModel() const { return _sourceModel; }

    /**
     * @brief Return the internal sort/filter proxy model.
     * @return Pointer to the proxy model
     */
    QSortFilterProxyModel* proxyModel() const { return _proxyModel; }

    /**
     * @brief Recursively collapse index and all its descendants up to a depth limit.
     * @param index Root index to collapse from
     * @param depth Maximum depth to collapse (-1 for unlimited)
     */
    void collapseRecursively(const QModelIndex& index, int depth = -1);

    /**
     * @brief Return whether the deepest expanded leaf under index is expanded.
     * @param index Root index to check
     * @param recursive Whether to check recursively
     * @return true if the leaf is expanded
     */
    bool isLeafExpanded(const QModelIndex& index, bool recursive = true) const;

    /**
     * @brief Return whether the given index is visible in the current viewport.
     * @param index Index to check
     * @return true if the index row is within the visible area
     */
    bool isIndexVisible(const QModelIndex& index) const;

    /**
     * @brief Assign a custom item delegate to the column of the given header type.
     * @param type Column header type identifier
     * @param delegate Delegate to install
     */
    void setColumnDelegate(int type, QStyledItemDelegate* delegate);

    /**
     * @brief Return the column index for a given header type.
     * @param headerType Header type identifier
     * @return Column index, or -1 if not found
     */
    int columnForHeaderType(int headerType) const;

    /**
     * @brief Show or hide columns identified by header types.
     * @param headerTypes List of header type identifiers
     * @param visible true to show, false to hide
     * @param exclusive If true, hide all other columns
     */
    void setColumnTypesVisible(const QList<int>& headerTypes, bool visible, bool exclusive = false);

    /**
     * @brief Show or hide columns by column index.
     * @param columns List of column indexes
     * @param visible true to show, false to hide
     * @param exclusive If true, hide all other columns
     */
    void setColumnsVisible(const QList<int>& columns, bool visible, bool exclusive = false);

public slots:
    /**
     * @brief Refresh the display of a set of columns.
     * @param columns Column indexes to refresh
     */
    virtual void refreshVisibleColumns(const QList<int>& columns);

    /**
     * @brief Refresh the display of a set of source model indexes.
     * @param indexes Source model indexes to refresh
     */
    virtual void refreshVisibleIndexes(const QModelIndexList& indexes);

    /**
     * @brief Refresh a single source model index.
     * @param sourceIndex Source model index to refresh
     */
    virtual void refreshIndex(const QModelIndex& sourceIndex);

    /** @brief Remove all items from the view model. */
    virtual void clear();

protected:
    /**
     * @brief Walk ancestors of the current index to find one with the given entity type.
     * @param entityMetadataType Entity type to search for
     * @return EntityMetadata of the first matching ancestor
     */
    EntityMetadata findCurrentParent(int entityMetadataType) const;

    /**
     * @brief Walk ancestors of index to find one with the given entity type.
     * @param index Starting index
     * @param entityMetadataType Entity type to search for
     * @return EntityMetadata of the first matching ancestor
     */
    EntityMetadata findFirstParent(const QModelIndex& index, int entityMetadataType) const;

    /**
     * @brief Return all ancestor indexes of index.
     * @param index Starting index
     * @return List of ancestor indexes from nearest to root
     */
    QModelIndexList findParents(const QModelIndex& index) const;

    /**
     * @brief Map a list of view indexes to their source model equivalents.
     * @param indexes Proxy model indexes to map
     * @return Corresponding source model indexes
     */
    QModelIndexList mapToSource(const QModelIndexList& indexes) const;

    /**
     * @brief Override to add application-specific items to the header context menu.
     * @param menu Menu to populate
     * @param globalPos Global cursor position when the menu was requested
     */
    virtual void addHeaderContextMenuItems(QMenu* menu, const QPoint& globalPos) { Q_UNUSED(menu) Q_UNUSED(globalPos) }

    /** @brief Log a model index at the given log level. */
    static void logIndex(const char* file, int lineNumber, Log::LogLevel level, const QModelIndex& index, const QString& text);
    /** @brief Test whether index matches value under role using flags, appending to foundIndexes. */
    static bool testMatch(const QModelIndex& index, int role, const QVariant &value, Qt::MatchFlags flags, QModelIndexList& foundIndexes);

private:
    AbstractItemModel* _sourceModel = nullptr;
    QSortFilterProxyModel* _proxyModel = nullptr;
    QMap<int, QStyledItemDelegate*> _columnDelegates;

    QAction* _actionColSettings = nullptr;
    QAction* _actionHideCol = nullptr;
    QAction* _actionAutoResizeCols = nullptr;
    QAction* _actionResetCols = nullptr;

    QPoint _contextMenuPos;

signals:
    /** @brief Emitted when an item is selected programmatically. */
    void itemProgramaticallySelected(const QModelIndex& index);
    /** @brief Emitted when the header is resized or reordered. */
    void headerChanged();
    /** @brief Emitted when the current selection changes. */
    void currentSelectionChanged();
    /**
     * @brief Emitted when the current index changes.
     * @param index The newly current index
     */
    void currentIndexChanged(const QModelIndex& index);

    /** @brief Emitted when an entity is added. */
    void entityAdded(const EntityMetadata& metadata);
    /** @brief Emitted when an entity is deleted. */
    void entityDeleted(const EntityMetadata& metadata);
    /** @brief Emitted when an entity is updated. */
    void entityUpdated(const EntityMetadata& metadata);

private slots:
    virtual void onHorizontalHeaderResized(int /*logicalIndex*/, int /*oldSize*/, int /*newSize*/);

    void onHeaderContextMenuRequested();
    void onColumnSettingsClicked();
    void onHideColumnClicked();
    void onAutoResizeColumnsClicked();
    void onResetColumnsClicked();
    void onCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);
};

#endif // TREEVIEWBASE_H
