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
class LIBKANOOPGUI_EXPORT AbstractModelItem
{
public:
    AbstractModelItem();
    AbstractModelItem(AbstractItemModel* model);
    AbstractModelItem(const EntityMetadata& entityMetadata, AbstractItemModel* model, const QUuid& uuid = QUuid());
    AbstractModelItem(const EntityMetadata& entityMetadata, const QUuid& uuid, AbstractItemModel* model);

    virtual ~AbstractModelItem() {
        qDeleteAll(_children);
    }

    // Overridable Properties
    virtual EntityMetadata entityMetadata() const { return _entityMetadata; }
    virtual EntityMetadata& entityMetadataRef() { return _entityMetadata; }
    virtual void setEntityMetadata(const EntityMetadata& metadata) { _entityMetadata = metadata; }
    virtual int entityType() const { return _entityMetadata.type(); }
    virtual QUuid uuid() const { return _uuid; }
    virtual QIcon icon() const { return _icon; }
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual void updateFromMetadata(const EntityMetadata& metadata);
    virtual void updateFromVariant(int headerType, const QVariant& value) { Q_UNUSED(headerType) Q_UNUSED(value) }

    // List
    class List : public QList<AbstractModelItem*>
    {
    public:
        AbstractModelItem* findByUuid(const QUuid& uuid) const
        {
            AbstractModelItem* result = nullptr;
            auto it = std::find_if(constBegin(), constEnd(), [uuid](AbstractModelItem* a) { return a->uuid() == uuid; } );
            if(it != constEnd()) {
                result = *it;
            }
            return result;
        }

        int indexOfUuid(const QUuid& uuid) const
        {
            int result = -1;
            auto it = std::find_if(constBegin(), constEnd(), [uuid](AbstractModelItem* a) { return a->uuid() == uuid; } );
            if(it != constEnd()) {
                result = std::distance(constBegin(), it);
            }
            return result;
        }

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
    };

    // Model Properties and Methods
    int row() const;

    AbstractModelItem* parent() const { return _parent; }
    AbstractModelItem* child(int row) const;
    List children() const { return _children; }
    List& childrenRef() { return _children; }
    List siblings() const { return _siblings; }

    int childCount(int entityType = 0) const;
    int childCountRecursive(int entityType = 0) const;

    AbstractModelItem* insertChild(int index, AbstractModelItem* child);
    AbstractModelItem* appendChild(AbstractModelItem* child);
    void deleteChild(AbstractModelItem* child);

    template <typename T>
    QList<T> findChildren() const
    {
        QList<T> result;
        for(AbstractModelItem* item : _children) {
            T candidate = dynamic_cast<T>(item);
            if(candidate != nullptr) {
                result.append(candidate);
            }
        }
        return result;
    }

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

    AbstractItemModel* model() const { return _model; }

protected:
    void setIcon(const QIcon& value) { _icon = value; }

private:
    EntityMetadata _entityMetadata;
    AbstractItemModel* _model;
    QUuid _uuid;
    AbstractModelItem* _parent;
    List _children;
    List _siblings;

    QIcon _icon;
};

#endif // ABSTRACTMODELITEM_H
