/******************************************************************************************
**
** abstractitemmodel.cpp
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
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#include "abstractitemmodel.h"
#include <Kanoop/log.h>

AbstractItemModel::AbstractItemModel(QObject *parent) :
    QAbstractItemModel(parent),
    LoggingBaseClass("itemmodel")
{
    commonInit();
}

AbstractItemModel::AbstractItemModel(const QString &loggingCategory, QObject *parent) :
    QAbstractItemModel(parent),
    LoggingBaseClass(loggingCategory)
{
    commonInit();
}

void AbstractItemModel::commonInit()
{
    AbstractItemModel::setObjectName(AbstractItemModel::metaObject()->className());
    Log::setCategoryLevel(LVL1().name(), Log::Info);
    Log::setCategoryLevel(LVL2().name(), Log::Info);
    Log::setCategoryLevel(LVL3().name(), Log::Info);
}

QModelIndex AbstractItemModel::index(int row, int column, const QModelIndex &parent) const
{
    logText(LVL_DEBUG, LVL2(), QString("%1  row: %2  col: %3  parent: [%4]").arg(__FUNCTION__).arg(row).arg(column).arg(toString(parent)));
    QModelIndex result;
    if(hasIndex(row, column, parent)) {
        if(parent.isValid() == false) {
            // root item
            if(row < _rootItems.count()) {
                result = createIndex(row, column, _rootItems.at(row));
            }
            else {
                logText(LVL_WARNING, "Request for invalid root index");
            }
        }
        else {
            // this is not a root level item
            AbstractModelItem* parentItem = static_cast<AbstractModelItem*>(parent.internalPointer());
            AbstractModelItem* childItem = static_cast<AbstractModelItem*>(parentItem->child(row));
            if(childItem != nullptr) {
                result = createIndex(row, column, childItem);
            }
        }
    }
    return result;
}

QModelIndex AbstractItemModel::parent(const QModelIndex &child) const
{
    logText(LVL_DEBUG, LVL2(), QString("%1  child: [%2]").arg(__FUNCTION__).arg(toString(child)));
    QModelIndex result;
    if(child.isValid()) {
        AbstractModelItem* childItem = static_cast<AbstractModelItem*>(child.internalPointer());
        if(childItem != nullptr && _rootItems.contains(childItem) == false) {
            AbstractModelItem* parentItem = childItem->parent();
            result = createIndex(childItem->row(), 0, parentItem);
        }
    }
    return result;
}

int AbstractItemModel::rowCount(const QModelIndex &parent) const
{
    logText(LVL_DEBUG, LVL2(), QString("%1  parent: [%2]").arg(__FUNCTION__).arg(toString(parent)));
    int result = 0;
    if(parent.isValid()) {
        result = static_cast<AbstractModelItem*>(parent.internalPointer())->children().count();
    }
    else {
        result = _rootItems.count();
    }
    return result;
}

int AbstractItemModel::columnCount(const QModelIndex &parent) const
{
    logText(LVL_DEBUG, LVL2(), QString("%1  parent: [%2]").arg(__FUNCTION__).arg(toString(parent)));

    return _columnHeadersIntMap.count() ? _columnHeadersIntMap.count() : 1;
//    int result = 1;
//    if(parent.isValid()) {
//        AbstractModelItem* parentItem = static_cast<AbstractModelItem*>(parent.internalPointer());
//        result = parentItem->siblings().count() + 1;
//    }
//    return result;
}

QVariant AbstractItemModel::data(const QModelIndex &index, int role) const
{
    logText(LVL_DEBUG, LVL2(), QString("%1  index: [%2]  role: %3").arg(__FUNCTION__).arg(toString(index)).arg(role));

    QVariant result;

    if(index.isValid() && index.internalPointer() != nullptr) {
        AbstractModelItem* item = static_cast<AbstractModelItem*>(index.internalPointer());
        result = item->data(index, role);
        if(result.isValid() == false) {
            switch(role) {
            case Qt::DisplayRole:
                result = item->data(index, role);
                break;
            case Qt::DecorationRole:
                result = item->icon();
                break;
            case KANOOP::EntityTypeRole:
                result = item->entityType();
                break;
            case KANOOP::EntityMetadataRole:
                result = QVariant::fromValue<EntityMetadata>(item->entityMetadata());
                break;
            default:
                break;
            }
        }
    }
    return result;
}

QVariant AbstractItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;
    switch(role) {
    case Qt::DisplayRole:
        result = orientation == Qt::Horizontal ? _columnHeadersIntMap.value(section).text() : _rowHeadersIntMap.value(section).text();
        break;
    default:
        break;
    }
    return result;
}

bool AbstractItemModel::removeRows(int row, int count, const QModelIndex& parentIndex)
{
    bool result = false;
    if(parentIndex.isValid()) {
        AbstractModelItem* parent = static_cast<AbstractModelItem*>(parentIndex.internalPointer());
        if(parent->childrenRef().count() >= row + count) {
            QList<AbstractModelItem*> deleteItems = parent->children().mid(row, count);
            beginRemoveRows(parentIndex, row, (row + count) - 1);
            parent->childrenRef().remove(row, count);
            endRemoveRows();
            qDeleteAll(deleteItems);
            result = true;
        }
    }
    else if(parentIndex.column() == 0 && _rootItems.count() >= row + count) {
        QList<AbstractModelItem*> deleteItems = _rootItems.mid(row, count);
        beginRemoveRows(parentIndex, row, (row + count) - 1);
        _rootItems.remove(row, count);
        endRemoveRows();
        qDeleteAll(deleteItems);
        result = true;
    }
    return result;
}

QModelIndexList AbstractItemModel::indexesOfEntityType(int type) const
{
    QModelIndexList result = match(index(0, 0, QModelIndex()), KANOOP::EntityTypeRole, type, -1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
    return result;
}

QModelIndexList AbstractItemModel::indexesOfEntity(int type, const QVariant &data, int role) const
{
    QModelIndexList result;
    QModelIndexList possibleMatches = indexesOfEntityType(type);
    for(const QModelIndex& index : possibleMatches) {
        if(index.data(role) == data) {
            result.append(index);
        }
    }
    return result;
}

QModelIndexList AbstractItemModel::indexesOfEntityUuid(const QUuid &uuid) const
{
    QModelIndexList result = match(index(0, 0, QModelIndex()), KANOOP::UUidRole, uuid, -1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
    return result;
}

QModelIndex AbstractItemModel::firstIndexOfEntityType(int type) const
{
    QModelIndexList matches = match(index(0, 0, QModelIndex()), KANOOP::EntityTypeRole, type, -1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
    QModelIndex result = matches.count() > 0 ? matches.first() : QModelIndex();
    return result;
}

QModelIndex AbstractItemModel::firstIndexOfEntity(int type, const QVariant &data, int role) const
{
    QModelIndex result;
    QModelIndexList possibleMatches = indexesOfEntityType(type);
    for(const QModelIndex& index : possibleMatches) {
        QVariant indexData = index.data(role);
        if(indexData == data) {
            result = index;
            break;
        }
    }
    return result;
}

QModelIndex AbstractItemModel::firstIndexOfEntityUuid(const QUuid &uuid) const
{
    QModelIndex result;
    QModelIndexList indexes = match(index(0, 0, QModelIndex()), KANOOP::UUidRole, uuid, 1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
    if(indexes.count() > 0) {
        result = indexes.first();
    }
    return result;
}

QModelIndex AbstractItemModel::firstIndexOfChildEntityType(const QModelIndex &parent, int type) const
{
    QModelIndexList matches = match(parent, KANOOP::EntityTypeRole, type, -1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
    QModelIndex result = matches.count() > 0 ? matches.first() : QModelIndex();
    return result;
}

QModelIndex AbstractItemModel::firstMatch(const QModelIndex& startSearchIndex, int role, const QVariant& value, Qt::MatchFlags flags) const
{
    QModelIndex result;
    QModelIndexList found = match(startSearchIndex, role, value, 1, flags);
    if(found.count() > 0) {
        result = found.first();
    }
    return result;
}

TableHeader::List AbstractItemModel::columnHeaders() const
{
    TableHeader::List result;
    QList<int> keys = _columnHeadersIntMap.keys();
    for(int key : keys) {
        result.append(_columnHeadersIntMap.value(key));
    }
    return result;
}

AbstractModelItem *AbstractItemModel::appendRootItem(AbstractModelItem *item)
{
    int row = rowCount(QModelIndex());
    QModelIndex insertIndex = index(row, 0, QModelIndex());
    beginInsertRows(insertIndex, row, row);
    _rootItems.append(item);
    endInsertRows();
    return item;
}

void AbstractItemModel::appendColumnHeader(int type, const QString &text)
{
    QString headerText = text.isEmpty() ? TableHeader::typeToString(type) : text;

    TableHeader header(type, headerText, Qt::Horizontal);
    _columnHeadersIntMap.insert(_columnHeadersIntMap.count(), header);
    _columnHeadersStringMap.insert(header.text(), header);
}

void AbstractItemModel::appendColumnHeader(int type, const QColor &columnTextColor, const QString &text)
{
    appendColumnHeader(type, text);
    _columnHeadersIntMap.setTextColorForType(type, columnTextColor);
    _columnHeadersIntMap.setTextColorForType(type, columnTextColor);
}

void AbstractItemModel::appendRowHeader(int type, const QString &value)
{
    QString text = value.isEmpty() ? TableHeader::typeToString(type) : value;

    TableHeader header(type, text, Qt::Vertical);
    _rowHeadersIntMap.insert(_columnHeadersIntMap.count(), header);
    _rowHeadersStringMap.insert(header.text(), header);
}

int AbstractItemModel::columnForHeader(int type) const
{
    int result = -1;
    for(int col = 0;col < _columnHeadersIntMap.count();col++) {
        if(_columnHeadersIntMap.value(col).type() == type) {
            result = col;
            break;
        }
    }
    return result;
}

QModelIndexList AbstractItemModel::getPersistentIndexes() const
{
    return persistentIndexList();
}

void AbstractItemModel::refresh(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    emit dataChanged(topLeft, bottomRight);
}

void AbstractItemModel::deleteRootItem(AbstractModelItem *item)
{
    if(_rootItems.contains(item) == true) {
        _rootItems.removeAll(item);
    }
    else {
        logText(LVL_WARNING, "Failed to find root item for deletion");
    }
    delete item;
}

void AbstractItemModel::deleteRootItems(const QUuid &uuid)
{
    QModelIndexList indexes = indexesOfEntityUuid(uuid);
    for(const QModelIndex& index : indexes) {
        AbstractModelItem* item = static_cast<AbstractModelItem*>(index.internalPointer());
        beginRemoveRows(index.parent(), index.row(), index.row());
        deleteRootItem(item);
        endRemoveRows();
    }
}

void AbstractItemModel::deleteRootItems(const EntityMetadata &metadata)
{
    QModelIndexList indexes = indexesOfEntity(metadata.type(), metadata.data(KANOOP::DataRole), KANOOP::DataRole);
    for(const QModelIndex& index : indexes) {
        AbstractModelItem* item = static_cast<AbstractModelItem*>(index.internalPointer());
        beginRemoveRows(index.parent(), index.row(), index.row());
        deleteRootItem(item);
        endRemoveRows();
    }
}

void AbstractItemModel::updateItemAtIndex(const QModelIndex &itemIndex, const EntityMetadata& metadata)
{
    AbstractModelItem* item = static_cast<AbstractModelItem*>(itemIndex.internalPointer());
    item->updateFromMetadata(metadata);
    emit dataChanged(itemIndex, index(itemIndex.row(), columnCount(itemIndex) - 1, itemIndex.parent()));
}

void AbstractItemModel::updateItemsAtIndexes(const QModelIndexList &indexes, const EntityMetadata &metadata)
{
    for(const QModelIndex& index : indexes) {
        updateItemAtIndex(index, metadata);
    }
}

void AbstractItemModel::emitRowChanged(const QModelIndex &rowIndex)
{
    QModelIndex firstColIndex = index(rowIndex.row(), 0, rowIndex.parent());
    QModelIndex lastColIndex = index(rowIndex.row(), columnCount(rowIndex) - 1, rowIndex.parent());
    emit dataChanged(firstColIndex, lastColIndex);
}

QString AbstractItemModel::toString(const QModelIndex &value)
{
    return QString("row: %1  col: %2  ptr: 0x%3").arg(value.row()).arg(value.column()).arg((qint64)value.constInternalPointer(), 0, 16);
}



