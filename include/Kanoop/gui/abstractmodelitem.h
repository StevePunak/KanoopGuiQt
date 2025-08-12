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
    class List : public QList<AbstractModelItem*>{};

    // Model Properties and Methods
    int row() const;

    AbstractModelItem* parent() const { return _parent; }
    AbstractModelItem* child(int row);
    List children() const { return _children; }
    List& childrenRef() { return _children; }
    List siblings() const { return _siblings; }

    int childCount(int entityType = 0) const;
    int childCountRecursive(int entityType = 0) const;

    AbstractModelItem* appendChild(AbstractModelItem* child);
    void deleteChild(AbstractModelItem* child);

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
