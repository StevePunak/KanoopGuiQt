/******************************************************************************************
**
** abstractitemmodel.h
**
** Extends the native Qt QAbstractItemModel to provide additional funtionality which
** I've found useful over the years.
**
** Most of the methods provide a default implementation of QAbstractItemModel and most
** other methods as self-explainitory if you are familiar with the Qt Abstract Model.
**
** It is intended to work with objects inherited from AbstractModelItem.
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#ifndef ABSTRACTITEMMODEL_H
#define ABSTRACTITEMMODEL_H
#include <QAbstractItemModel>
#include <Kanoop/gui/abstractmodelitem.h>
#include <Kanoop/gui/tableheader.h>
#include <Kanoop/utility/loggingbaseclass.h>

/**
 * @brief Extended QAbstractItemModel providing EntityMetadata-based item lookup and header management.
 *
 * Subclass this together with AbstractModelItem to build tree, table, or list models.
 * Column and row headers are managed internally; entity lookup helpers search by type or UUID.
 */
class LIBKANOOPGUI_EXPORT AbstractItemModel : public QAbstractItemModel,
                                              public LoggingBaseClass
{
    Q_OBJECT
public:
    /** @brief Construct with an optional parent. */
    AbstractItemModel(QObject* parent = nullptr);

    /**
     * @brief Construct with a logging category and optional parent.
     * @param loggingCategory Category name used for log output
     * @param parent Optional QObject parent
     */
    AbstractItemModel(const QString& loggingCategory, QObject* parent = nullptr);

    /** @brief Destructor — deletes all root items. */
    virtual ~AbstractItemModel() {
        qDeleteAll(_rootItems);
    }

    // EntityMetadata Helpers
    /**
     * @brief Return all indexes whose item has the given entity type.
     * @param type Entity type to search for
     * @return List of matching model indexes
     */
    virtual QModelIndexList indexesOfEntityType(int type) const;

    /**
     * @brief Return all indexes matching entity type and a role value.
     * @param type Entity type to search for
     * @param data Value to match against the given role
     * @param role Model role to compare (default Qt::DisplayRole)
     * @return List of matching model indexes
     */
    virtual QModelIndexList indexesOfEntity(int type, const QVariant &data, int role = Qt::DisplayRole) const;

    /**
     * @brief Return all indexes whose item UUID matches.
     * @param uuid UUID to search for
     * @return List of matching model indexes
     */
    virtual QModelIndexList indexesOfEntityUuid(const QUuid& uuid) const;

    /**
     * @brief Return the first index whose item has the given entity type.
     * @param type Entity type to search for
     * @return First matching index, or invalid index if not found
     */
    virtual QModelIndex firstIndexOfEntityType(int type) const;

    /**
     * @brief Return the first index matching entity type and a role value.
     * @param type Entity type to search for
     * @param data Value to match against the given role
     * @param role Model role to compare (default Qt::DisplayRole)
     * @return First matching index, or invalid index if not found
     */
    virtual QModelIndex firstIndexOfEntity(int type, const QVariant &data, int role = Qt::DisplayRole) const;

    /**
     * @brief Return the first index whose item UUID matches.
     * @param uuid UUID to search for
     * @return First matching index, or invalid index if not found
     */
    virtual QModelIndex firstIndexOfEntityUuid(const QUuid& uuid) const;

    /**
     * @brief Return the first child index under parent with the given entity type.
     * @param parent Parent index to search under
     * @param type Entity type to search for
     * @param recursive Whether to search recursively (default true)
     * @return First matching child index, or invalid index if not found
     */
    virtual QModelIndex firstIndexOfChildEntityType(const QModelIndex& parent, int type, bool recursive = true) const;

    /**
     * @brief Return the first child index under parent whose UUID matches.
     * @param parent Parent index to search under
     * @param uuid UUID to search for
     * @param recursive Whether to search recursively (default true)
     * @return First matching child index, or invalid index if not found
     */
    virtual QModelIndex firstIndexOfChildEntityUuid(const QModelIndex& parent, const QUuid& uuid, bool recursive = true) const;

    /**
     * @brief Return the first match starting from startSearchIndex.
     * @param startSearchIndex Index to begin searching from
     * @param role Model role to compare
     * @param value Value to match
     * @param flags Match flags
     * @return First matching index, or invalid index if not found
     */
    virtual QModelIndex firstMatch(const QModelIndex& startSearchIndex, int role, const QVariant& value, Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const;

    /**
     * @brief Return the first match anywhere in the model.
     * @param role Model role to compare
     * @param value Value to match
     * @param flags Match flags
     * @return First matching index, or invalid index if not found
     */
    virtual QModelIndex firstMatch(int role, const QVariant& value, Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const;

    /**
     * @brief Return child indexes under parent, optionally filtered by entity type.
     * @param parent Parent index
     * @param type Entity type filter (-1 to return all types)
     * @param recursive Whether to search recursively (default true)
     * @return List of matching child indexes
     */
    virtual QModelIndexList childIndexes(const QModelIndex& parent, int type = -1, bool recursive = true) const;

    /**
     * @brief Return all column headers as a sorted list.
     * @return List of TableHeader objects ordered by column index
     */
    TableHeader::List columnHeaders() const;

    /**
     * @brief Return the column header for a given section.
     * @param section Column section index
     * @return TableHeader for that section
     */
    TableHeader columnHeader(int section) const { return _columnHeaders.value(section); }

    /**
     * @brief Return the row header for a given row.
     * @param row Row index
     * @return TableHeader for that row
     */
    TableHeader rowHeader(int row) const { return _rowHeaders.value(row); }

    /**
     * @brief Return the column index for a header type.
     * @param type Header type identifier
     * @return Column index, or -1 if not found
     */
    int columnForHeader(int type) const;

    /**
     * @brief Return all persistent model indexes.
     * @return List of persistent indexes currently held by the model
     */
    QModelIndexList getPersistentIndexes() const;

    /**
     * @brief Emit dataChanged for the rectangular region from topLeft to bottomRight.
     * @param topLeft Top-left corner of the changed region
     * @param bottomRight Bottom-right corner of the changed region
     */
    virtual void refresh(const QModelIndex& topLeft, const QModelIndex& bottomRight);

    // QAbstractItemModel interface
    /** @brief Return the model index for the item at row/column under parent. */
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
#if NEEDS_TESTING
    virtual QModelIndex sibling(int row, int column, const QModelIndex& idx) const override;
#endif
    /** @brief Return the parent index of a child index. */
    virtual QModelIndex parent(const QModelIndex &child) const override;
    /** @brief Return the number of rows under parent. */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    /** @brief Return the number of columns under parent. */
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    /** @brief Return the data for the given index and role. */
    virtual QVariant data(const QModelIndex &index, int role) const override;
    /** @brief Return header data for the given section, orientation, and role. */
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    /** @brief Remove count rows starting at row under parentIndex. */
    virtual bool removeRows(int row, int count, const QModelIndex& parentIndex) override;
    /** @brief Return true if parent has child items. */
    virtual bool hasChildren(const QModelIndex& parent) const override;
    /** @brief Set header data for the given section, orientation, and role. */
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role) override;

    /**
     * @brief Show or hide the column whose header has the given type.
     * @param type Column header type identifier
     * @param visible Whether to make the column visible
     */
    void setColumnHeaderVisible(int type, bool visible);

    /**
     * @brief Format a QModelIndex as a debug string.
     * @param index Index to format
     * @param includeText Whether to include display text (default false)
     * @return Human-readable string describing the index
     */
    static QString indexToString(const QModelIndex& index, bool includeText = false);

protected:
    /** @brief Return the list of root items. */
    AbstractModelItem::List rootItems() const { return _rootItems; }
    /** @brief Return a mutable reference to the root items list. */
    AbstractModelItem::List& rootItemsRef() { return _rootItems; }
    /** @brief Return a const copy of the root items list. */
    const AbstractModelItem::List rootItemsConst() const { return _rootItems; }
    /** @brief Return the number of root items. */
    int rootItemCount() const { return _rootItems.count(); }

    /**
     * @brief Insert an item at the given row among root items.
     * @param row Insertion row index
     * @param item Item to insert
     * @return Pointer to the inserted item
     */
    AbstractModelItem* insertRootItem(int row, AbstractModelItem* item);

    /**
     * @brief Append an item to the root items list.
     * @param item Item to append
     * @return Pointer to the appended item
     */
    AbstractModelItem* appendRootItem(AbstractModelItem* item);

    /**
     * @brief Append multiple items to the root items list.
     * @param items Items to append
     */
    void appendRootItems(QList<AbstractModelItem*> items);

    /**
     * @brief Append a column header with the given type and display text.
     * @param type Column type identifier
     * @param text Header display text
     */
    void appendColumnHeader(int type, const QString& text);

    /**
     * @brief Append a column header with a custom text color.
     * @param type Column type identifier
     * @param columnTextColor Text color for cells in this column
     * @param text Header display text
     */
    void appendColumnHeader(int type, const QColor& columnTextColor, const QString& text);

    /**
     * @brief Insert a column header at the given index.
     * @param type Column type identifier
     * @param index Position to insert at
     * @param text Header display text
     */
    void insertColumnHeader(int type, int index, const QString& text);

    /** @brief Append a column header (ad-hoc alias for appendColumnHeader). */
    void appendAdHocColumnHeader(int type, const QString& value) { appendColumnHeader(type, value); }

    /**
     * @brief Delete the column header at the given section.
     * @param section Section index to remove
     */
    void deleteColumnHeader(int section);

    /**
     * @brief Append a row header with optional display text.
     * @param type Row type identifier
     * @param value Optional display text
     */
    void appendRowHeader(int type, const QString& value = QString());

    /** @brief Append a row header (ad-hoc alias for appendRowHeader). */
    void appendAdHocRowHeader(int type, const QString& value) { appendRowHeader(type, value); }

    /**
     * @brief Change the display text of a column header.
     * @param section Section index to update
     * @param text New display text
     */
    void setColumnHeaderText(int section, const QString& text);

    /**
     * @brief Associate an EntityMetadata with a column header type.
     * @param type Column type identifier
     * @param metadata Metadata to associate
     */
    void setColumnHeaderEntityMetadata(int type, const EntityMetadata& metadata);

    /**
     * @brief Retrieve the EntityMetadata for a column header type.
     * @param type Column type identifier
     * @return Associated EntityMetadata
     */
    EntityMetadata columnEntityMetadata(int type) const;

    /**
     * @brief Set the text color for all cells in the column of the given type.
     * @param type Column type identifier
     * @param color Text color to apply
     */
    void setColumnTextColor(int type, const QColor& color);

    /**
     * @brief Delete a root item from the model.
     * @param item Root item to delete
     */
    void deleteRootItem(AbstractModelItem* item);

    /**
     * @brief Delete all root items with the given UUID.
     * @param uuid UUID to match
     */
    void deleteRootItems(const QUuid& uuid);

    /**
     * @brief Delete all root items matching the given EntityMetadata.
     * @param metadata Metadata to match
     */
    void deleteRootItems(const EntityMetadata& metadata);

    /**
     * @brief Delete any item (at any level) with the given UUID.
     * @param uuid UUID to match
     */
    void deleteItem(const QUuid& uuid);

    /**
     * @brief Update the item at itemIndex with new metadata and emit dataChanged.
     * @param itemIndex Index of the item to update
     * @param metadata New metadata to apply
     */
    void updateItemAtIndex(const QModelIndex& itemIndex, const EntityMetadata &metadata);

    /**
     * @brief Update items at multiple indexes with new metadata.
     * @param indexes Indexes of items to update
     * @param metadata New metadata to apply
     */
    void updateItemsAtIndexes(const QModelIndexList& indexes, const EntityMetadata &metadata);

    /** @brief Emit dataChanged for the entire model. */
    void refreshAll();

    /**
     * @brief Find all items of type T in the model (root and their children).
     * @tparam T Pointer type of items to find (must derive from AbstractModelItem)
     * @return List of matching items cast to T
     */
    template <typename T>
    QList<T> findItems() const
    {
        QList<T> result;
        for(AbstractModelItem* item : rootItems()) {
            T candidate = dynamic_cast<T>(item);
            if(candidate != nullptr) {
                result.append(candidate);
            }
            result.append(item->findChildren<T>(true));
        }
        return result;
    }

    /**
     * @brief Find the first direct child of parentIndex matching a role/value pair.
     * @param parentIndex Parent index to search under
     * @param value Value to match
     * @param role Role to compare
     * @return First matching child index, or invalid index
     */
    QModelIndex findFirstDirectChild(const QModelIndex& parentIndex, const QVariant& value, int role) const;

    /** @brief Return the column headers as an int-keyed map. */
    TableHeader::IntMap columnHeadersIntMap() const { return _columnHeaders; }
    /** @brief Return the row headers as an int-keyed map. */
    TableHeader::IntMap rowHeadersIntMap() const { return _rowHeaders; }

    /**
     * @brief Emit dataChanged for all columns of the given row index.
     * @param rowIndex Row index whose data changed
     */
    void emitRowChanged(const QModelIndex &rowIndex);

public:
    /**
     * @brief Format a QModelIndex as a debug string (static alias).
     * @param index Index to format
     * @param includeText Whether to include display text
     * @return Human-readable string
     */
    static QString toString(const QModelIndex& index, bool includeText = false);

private:
    /** @brief Shared constructor initialization. */
    void commonInit();

    AbstractModelItem::List _rootItems;

    TableHeader::IntMap _columnHeaders;
    TableHeader::IntMap _rowHeaders;

    friend class AbstractModelItem;

signals:
    /** @brief Emitted after an entity is added to the model. */
    void entityAdded(const EntityMetadata& metadata);
    /** @brief Emitted after an entity is deleted from the model. */
    void entityDeleted(const EntityMetadata& metadata);
    /** @brief Emitted after an entity in the model is updated. */
    void entityUpdated(const EntityMetadata& metadata);

public slots:
    /** @brief Remove all root items from the model. */
    virtual void clear();
    /** @brief Handle an entity-added event (no-op by default). */
    virtual void addEntity(const EntityMetadata& metadata) { Q_UNUSED(metadata) }
    /** @brief Handle an entity-deleted event (no-op by default). */
    virtual void deleteEntity(const EntityMetadata& metadata) { Q_UNUSED(metadata) }
    /** @brief Handle an entity-updated event (no-op by default). */
    virtual void updateEntity(const EntityMetadata& metadata) { Q_UNUSED(metadata) }
};

#endif // ABSTRACTITEMMODEL_H
