/******************************************************************************************
**
** tableviewbase.h
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#ifndef TABLEVIEWBASE_H
#define TABLEVIEWBASE_H
#include <Kanoop/entitymetadata.h>
#include <Kanoop/utility/loggingbaseclass.h>

#include <QTableView>
#include <Kanoop/gui/libkanoopgui.h>

class QStyledItemDelegate;
class QSortFilterProxyModel;
class AbstractItemModel;

/**
 * @brief QTableView subclass integrating AbstractItemModel with entity metadata helpers.
 *
 * TableViewBase wraps the source model in a QSortFilterProxyModel and provides helpers
 * for accessing EntityMetadata at positions, navigating by UUID, managing row delegates,
 * and restoring column/row header states.
 */
class LIBKANOOPGUI_EXPORT TableViewBase : public QTableView,
                                          public LoggingBaseClass
{
    Q_OBJECT

public:
    /**
     * @brief Construct with an optional parent widget.
     * @param parent Optional QWidget parent
     */
    explicit TableViewBase(QWidget* parent = nullptr);

    /** @brief Destructor. */
    virtual ~TableViewBase();

    /**
     * @brief Set the model, wrapping it in an internal sort/filter proxy.
     * @param model Model to display (should derive from AbstractItemModel)
     */
    virtual void setModel(QAbstractItemModel* model) override;

    /**
     * @brief Return the entity type of the item at a view position.
     * @param pos View-local position
     * @return Entity type integer, or -1 if no item at that position
     */
    int entityTypeAtPos(const QPoint& pos);

    /**
     * @brief Return the EntityMetadata of the currently selected item.
     * @return EntityMetadata for the current selection
     */
    EntityMetadata currentMetadata() const;

    /**
     * @brief Return the EntityMetadata of the item at a view position.
     * @param pos View-local position
     * @return EntityMetadata at that position
     */
    EntityMetadata metadataAtPos(const QPoint& pos) const;

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
     * @brief Delete the row identified by the given model index.
     * @param index Model index of the row to delete
     */
    virtual void deleteRow(const QModelIndex& index);

    /**
     * @brief Emit entityAdded() for the given metadata (override to customise).
     * @param metadata Metadata describing the entity to add
     */
    virtual void addRow(const EntityMetadata& metadata) { emit entityAdded(metadata); }

    /**
     * @brief Return the number of rows currently in the view.
     * @return Row count
     */
    int rowCount() const;

    /**
     * @brief Return model indexes for all rows in the view.
     * @return List of all row indexes (first column of each row)
     */
    QModelIndexList allRows() const;

    /**
     * @brief Return the first model index where the given role matches a value.
     * @param needle Value to search for
     * @param role Model role to compare
     * @return First matching index, or invalid index if not found
     */
    QModelIndex findFirstMatch(const QVariant& needle, int role) const;

    /**
     * @brief Select the item with the given UUID and optionally scroll to it.
     * @param uuid UUID of the item to select
     * @param scrollHint How to scroll to make the item visible
     */
    void setCurrentUuid(const QUuid& uuid, ScrollHint scrollHint = EnsureVisible);

    /**
     * @brief Return whether the given model index is currently visible in the viewport.
     * @param index Model index to check
     * @return true if the index is within the visible area
     */
    bool isIndexVisible(const QModelIndex& index) const;

    /** @brief Restore both horizontal and vertical header states from settings. */
    void restoreHeaderStates();
    /** @brief Restore horizontal header state (column widths/order) from settings. */
    void restoreHorizontalHeaderState();
    /** @brief Restore vertical header state (row heights) from settings. */
    void restoreVerticalHeaderState();

    /**
     * @brief Assign a custom item delegate to the column of the given header type.
     * @param type Column header type identifier
     * @param delegate Delegate to install
     */
    void setColumnDelegate(int type, QStyledItemDelegate* delegate);

public slots:
    /** @brief Remove all rows from the view model. */
    void clear();

private:
    AbstractItemModel* _sourceModel;
    QSortFilterProxyModel* _proxyModel;
    QMap<int, QStyledItemDelegate*> _columnDelegates;

    QAction* _actionColSettings = nullptr;
    QAction* _actionHideCol = nullptr;
    QAction* _actionAutoResizeCols = nullptr;
    QAction* _actionResetCols = nullptr;

    QPoint _contextMenuPoint;

signals:
    /** @brief Emitted when the horizontal header is resized. */
    void horizontalHeaderChanged();
    /** @brief Emitted when the vertical header is resized. */
    void verticalHeaderChanged();

    /** @brief Emitted when the current selection changes. */
    void currentSelectionChanged();
    /**
     * @brief Emitted when the current index changes.
     * @param current The newly current index
     * @param previous The previously current index
     */
    void currentIndexChanged(const QModelIndex& current, const QModelIndex& previous);

    /** @brief Emitted when a row is to be added with the given metadata. */
    void entityAdded(const EntityMetadata& metadata);
    /** @brief Emitted when an entity row is deleted. */
    void entityDeleted(const EntityMetadata& metadata);
    /** @brief Emitted when an entity row is updated. */
    void entityUpdated(const EntityMetadata& metadata);

protected slots:
    /** @brief Internal override forwarding current-index changes to currentIndexChanged(). */
    virtual void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;

private slots:
    virtual void onHorizontalHeaderResized(int /*logicalIndex*/, int /*oldSize*/, int /*newSize*/);
    virtual void onVerticalHeaderResized(int /*logicalIndex*/, int /*oldSize*/, int /*newSize*/);

    void onHeaderContextMenuRequested(const QPoint& pos);
    void onColumnSettingsClicked();
    void onHideColumnClicked();
    void onAutoResizeColumnsClicked();
    void onResetColumnsClicked();
};

#endif // TABLEVIEWBASE_H
