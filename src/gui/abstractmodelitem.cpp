/******************************************************************************************
**
** abstractmodelitem.cpp
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
#include "abstractmodelitem.h"
#include "resources.h"
#include "guitypes.h"

#include <QModelIndex>


AbstractModelItem::AbstractModelItem() :
    _model(nullptr),
    _parent(nullptr) {}

AbstractModelItem::AbstractModelItem(const EntityMetadata &entityMetadata, AbstractItemModel *model, const QUuid& uuid) :
    _entityMetadata(entityMetadata), _model(model), _uuid(uuid), _parent(nullptr)
{
    if(entityMetadata.iconId() != 0) {
        _icon = Resources::getIcon(entityMetadata.iconId());
    }
}

QVariant AbstractModelItem::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index)
    QVariant result;

    switch(role) {
    case Qt::DecorationRole:
        if(index.column() == 0) {
            result = _icon;
        }
        break;

    case KANOOP::DataRole:
        result = _entityMetadata.data(KANOOP::DataRole);
        break;

    case KANOOP::EntityTypeRole:
        result = _entityMetadata.type();
        break;

    case KANOOP::UUidRole:
        result = _entityMetadata.data(KANOOP::UUidRole);
        break;

    case KANOOP::EntityMetadataRole:
        result = QVariant::fromValue<EntityMetadata>(_entityMetadata);
        break;

    case KANOOP::MetadataTypeRole:
        result = _entityMetadata.type();
        break;

    default:
        break;
    }

    return result;
}

void AbstractModelItem::updateFromMetadata(const EntityMetadata& metadata)
{
    if(_entityMetadata.hasData(KANOOP::DataRole)) {
        _entityMetadata.setData(metadata.data(), KANOOP::DataRole);
    }
}

int AbstractModelItem::row() const
{
    int result = 0;
    if(_parent != nullptr) {
        result = _parent->_children.indexOf(this);
    }
    return result;
}

AbstractModelItem *AbstractModelItem::appendChild(AbstractModelItem *child)
{
    child->_parent = this;
    _children.append(child);
    return child;
}

void AbstractModelItem::deleteChild(AbstractModelItem *child)
{
    _children.removeAll(child);
    delete child;
}

AbstractModelItem *AbstractModelItem::child(int row)
{
    AbstractModelItem* result = row < _children.count() ? _children.at(row) : nullptr;
    return result;
}
