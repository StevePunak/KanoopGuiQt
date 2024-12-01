/******************************************************************************************
**
** abstractmodelitem.h
**
** Copyright (C) Tesseract Engineering, Inc - All Rights Reserved
**
** This source code is protected under international copyright law.  All rights
** reserved and protected by the copyright holders.
**
** This file is confidential and only available to authorized individuals with the
** permission of the copyright holders.  If you encounter this file and do not have
** permission, please contact the copyright holders and delete this file.
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

    virtual ~AbstractModelItem() {
        qDeleteAll(_children);
    }

    // Overridable Properties
    virtual EntityMetadata entityMetadata() const { return _entityMetadata; }
    virtual EntityMetadata& entityMetadataRef() { return _entityMetadata; }
    virtual int entityType() const { return _entityMetadata.type(); }
    virtual QUuid uuid() const { return _uuid; }
    virtual QIcon icon() const { return _icon; }
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual void updateFromMetadata(const EntityMetadata& metadata);

    // List
    class List : public QList<AbstractModelItem*>{};

    // Model Properties and Methods
    int row() const;

    AbstractModelItem* parent() const { return _parent; }
    AbstractModelItem* child(int row);
    List children() const { return _children; }
    List& childrenRef() { return _children; }
    List siblings() const { return _siblings; }

    AbstractModelItem* appendChild(AbstractModelItem* child);
    void deleteChild(AbstractModelItem* child);

    AbstractItemModel* model() const { return _model; }

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
