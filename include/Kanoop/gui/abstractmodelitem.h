/******************************************************************************************
**
** abstractmodelitem.h
**
** Provides a base class for items which use the AbstractItemModel class in this library
** and the EntityMetadata class in KanoopCommonQt.
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Sun Oct 22 17:41:53 2023
**
******************************************************************************************/
#ifndef ABSTRACTMODELITEM_H
#define ABSTRACTMODELITEM_H
#include <QAbstractAnimation>
#include <QIcon>
#include <QUuid>
#include <Kanoop/entitymetadata.h>
#include <Kanoop/gui/libkanoopgui.h>

class AbstractItemModel;

/**
 * @brief Base class for items managed by AbstractItemModel.
 *
 * Each item holds an EntityMetadata descriptor, a UUID, an optional icon, and
 * references to its parent model, parent item, and child items.  Subclass this
 * to attach domain-specific data and override data() to supply display values.
 */
class LIBKANOOPGUI_EXPORT AbstractModelItem
{
public:
    /** @brief Default constructor — creates an unowned, untyped item. */
    AbstractModelItem();

    /**
     * @brief Construct an item owned by a model with no metadata.
     * @param model Model that owns this item
     */
    AbstractModelItem(AbstractItemModel* model);

    /**
     * @brief Construct an item with metadata, a model, and an optional UUID.
     * @param entityMetadata Metadata describing the entity
     * @param model Model that owns this item
     * @param uuid Optional UUID for this item
     */
    AbstractModelItem(const EntityMetadata& entityMetadata, AbstractItemModel* model, const QUuid& uuid = QUuid());

    /**
     * @brief Construct an item with metadata, a UUID, and a model.
     * @param entityMetadata Metadata describing the entity
     * @param uuid UUID for this item
     * @param model Model that owns this item
     */
    AbstractModelItem(const EntityMetadata& entityMetadata, const QUuid& uuid, AbstractItemModel* model);

    /** @brief Destructor — deletes all child items. */
    virtual ~AbstractModelItem() {
        qDeleteAll(_children);
    }

    // Overridable Properties
    /** @brief Return the entity metadata for this item. */
    virtual EntityMetadata entityMetadata() const { return _entityMetadata; }
    /** @brief Return a mutable reference to the entity metadata. */
    virtual EntityMetadata& entityMetadataRef() { return _entityMetadata; }
    /**
     * @brief Set the entity metadata for this item.
     * @param metadata New metadata
     */
    virtual void setEntityMetadata(const EntityMetadata& metadata) { _entityMetadata = metadata; }
    /** @brief Return the entity type integer from this item's metadata. */
    virtual int entityType() const { return _entityMetadata.type(); }
    /** @brief Return the UUID for this item. */
    virtual QUuid uuid() const { return _uuid; }
    /** @brief Return the icon for this item. */
    virtual QIcon icon() const { return _icon; }
    /**
     * @brief Return display or decoration data for a model index.
     * @param index Model index being queried
     * @param role Qt item data role
     * @return Data for the given role
     */
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    /**
     * @brief Update this item's data from new entity metadata.
     * @param metadata New metadata to apply
     */
    virtual void updateFromMetadata(const EntityMetadata& metadata);
    /**
     * @brief Update a single cell from a raw variant value (no-op by default).
     * @param headerType Column header type
     * @param value New cell value
     */
    virtual void updateFromVariant(int headerType, const QVariant& value) { Q_UNUSED(headerType) Q_UNUSED(value) }

    /**
     * @brief A list of AbstractModelItem pointers with UUID and entity-type search helpers.
     */
    class List : public QList<AbstractModelItem*>
    {
    public:
        /**
         * @brief Find the first item with the given UUID.
         * @param uuid UUID to search for
         * @return Matching item pointer, or nullptr if not found
         */
        AbstractModelItem* findByUuid(const QUuid& uuid) const
        {
            AbstractModelItem* result = nullptr;
            auto it = std::find_if(constBegin(), constEnd(), [uuid](AbstractModelItem* a) { return a->uuid() == uuid; } );
            if(it != constEnd()) {
                result = *it;
            }
            return result;
        }

        /**
         * @brief Return the list index of the first item with the given UUID.
         * @param uuid UUID to search for
         * @return Index of the matching item, or -1 if not found
         */
        int indexOfUuid(const QUuid& uuid) const
        {
            int result = -1;
            auto it = std::find_if(constBegin(), constEnd(), [uuid](AbstractModelItem* a) { return a->uuid() == uuid; } );
            if(it != constEnd()) {
                result = std::distance(constBegin(), it);
            }
            return result;
        }

        /**
         * @brief Return the list index of the first item with the given entity type.
         * @param entityType Entity type to search for
         * @return Index of the matching item, or -1 if not found
         */
        int firstIndexOfEntityType(int entityType) const
        {
            int result = -1;
            for(int i = 0;i < count();i++) {
                AbstractModelItem* item = this->at(i);
                if(item->entityType() == entityType) {
                    result = i;
                    break;
                }
            }
            return result;
        }

        /**
         * @brief Return the list index of the last item with the given entity type.
         * @param entityType Entity type to search for
         * @return Index of the matching item, or -1 if not found
         */
        int lastIndexOfEntityType(int entityType) const
        {
            int result = -1;
            for(int i = 0;i < count();i++) {
                AbstractModelItem* item = this->at(i);
                if(item->entityType() == entityType) {
                    result = i;
                }
            }
            return result;
        }

        /**
         * @brief Find all child items castable to type T, optionally recursing.
         * @tparam T Pointer type to find
         * @param recursive Whether to recurse into children (default true)
         * @return List of matching items cast to T
         */
        template <typename T>
        QList<T> findChildItems(bool recursive = true) const
        {
            QList<T> result;
            for(AbstractModelItem* item : *this) {
                T candidate = dynamic_cast<T>(item);
                if(candidate != nullptr) {
                    result.append(candidate);
                }

                if(recursive) {
                    result.append(item->findChildren<T>(recursive));
                }
            }
            return result;
        }
    };

    // Model Properties and Methods
    /**
     * @brief Return the row index of this item within its parent's children.
     * @return Row index, or 0 if this item has no parent
     */
    int row() const;

    /** @brief Return this item's parent item, or nullptr if it is a root item. */
    AbstractModelItem* parent() const { return _parent; }

    /**
     * @brief Return the child item at the given row.
     * @param row Row index
     * @return Child item pointer, or nullptr if row is out of range
     */
    AbstractModelItem* child(int row) const;

    /** @brief Return the list of child items. */
    List children() const { return _children; }
    /** @brief Return a mutable reference to the child items list. */
    List& childrenRef() { return _children; }
    /** @brief Return the list of sibling items (all children of this item's parent). */
    List siblings() const;

    /**
     * @brief Return the number of direct children, optionally filtered by entity type.
     * @param entityType Entity type filter (0 = count all types)
     * @return Count of matching children
     */
    int childCount(int entityType = 0) const;

    /**
     * @brief Return the total number of descendants, optionally filtered by entity type.
     * @param entityType Entity type filter (0 = count all types)
     * @return Total count of matching descendants
     */
    int childCountRecursive(int entityType = 0) const;

    /**
     * @brief Insert a child item at the given index.
     * @param index Position to insert at
     * @param child Child item to insert
     * @return Pointer to the inserted child
     */
    AbstractModelItem* insertChild(int index, AbstractModelItem* child);

    /**
     * @brief Append a child item.
     * @param child Child item to append
     * @return Pointer to the appended child
     */
    AbstractModelItem* appendChild(AbstractModelItem* child);

    /**
     * @brief Delete a specific child item.
     * @param child Child item to remove and delete
     */
    void deleteChild(AbstractModelItem* child);

    /** @brief Delete and remove all child items. */
    void deleteAllChildren();

    /**
     * @brief Find all direct (or recursive) children castable to type T.
     * @tparam T Pointer type to find
     * @param recursive Whether to recurse into grandchildren (default false)
     * @return List of matching children cast to T
     */
    template <typename T>
    QList<T> findChildren(bool recursive = false) const
    {
        QList<T> result = _children.findChildItems<T>(recursive);
        return result;
    }

    /**
     * @brief Return the first direct child castable to type T.
     * @tparam T Pointer type to find
     * @return First matching child cast to T, or nullptr if none
     */
    template <typename T>
    T firstChild() const
    {
        T result = nullptr;
        for(AbstractModelItem* item : _children) {
            T candidate = dynamic_cast<T>(item);
            if(candidate != nullptr) {
                result = candidate;
                break;
            }
        }
        return result;
    }

    /** @brief Return the model that owns this item. */
    AbstractItemModel* model() const { return _model; }

protected:
    /**
     * @brief Set the icon for this item.
     * @param value Icon to assign
     */
    void setIcon(const QIcon& value) { _icon = value; }

private:
    EntityMetadata _entityMetadata;
    AbstractItemModel* _model = nullptr;
    QUuid _uuid;
    AbstractModelItem* _parent = nullptr;
    List _children;

    QIcon _icon;
};

#endif // ABSTRACTMODELITEM_H
