/******************************************************************************************
**
** abstractitemmodel.cpp
**
** Moved from my Tesseract Engineering repo to open-source
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

void AbstractItemModel::clear()
{
    beginResetModel();
    qDeleteAll(_rootItems);
    _rootItems.clear();
    endResetModel();
}

QModelIndex AbstractItemModel::index(int row, int column, const QModelIndex &parent) const
{
    logText(LVL_DEBUG, LVL3(), QString("%1  row: %2  col: %3  parent: [%4]").arg(__FUNCTION__).arg(row).arg(column).arg(toString(parent)));
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
    logText(LVL_DEBUG, LVL3(), QString("func: %1  child: [%2]").arg(__FUNCTION__).arg(toString(child)));
    QModelIndex result;
    if(child.isValid()) {
        AbstractModelItem* childItem = static_cast<AbstractModelItem*>(child.internalPointer());
        if(childItem != nullptr && _rootItems.contains(childItem) == false) {
            AbstractModelItem* parentItem = childItem->parent();
            if(parentItem != nullptr) {
                result = createIndex(parentItem->row(), 0, parentItem);
            }
        }
    }
    logText(LVL_DEBUG, LVL2(), QString("func: %1 for [%2] returns parent: [%3]")
            .arg(__FUNCTION__)
            .arg(toString(child))
            .arg(toString(result)));
    return result;
}

int AbstractItemModel::rowCount(const QModelIndex &parent) const
{
    logText(LVL_DEBUG, LVL3(), QString("%1  parent: [%2]").arg(__FUNCTION__).arg(toString(parent)));
    int result = 0;
    // We don't support children other than column 0
    if(parent.isValid()) {
        if(parent.column() == 0) {
            result = static_cast<AbstractModelItem*>(parent.internalPointer())->children().count();
        }
    }
    else {
        result = _rootItems.count();
    }
    logText(LVL_DEBUG, LVL3(), QString("%1  returns %2").arg(__FUNCTION__).arg(result));
    return result;
}

int AbstractItemModel::columnCount(const QModelIndex &parent) const
{
    logText(LVL_DEBUG, LVL3(), QString("%1  parent: [%2]").arg(__FUNCTION__).arg(toString(parent)));

    return qMax(1, columnHeadersIntMap().count());
}

QVariant AbstractItemModel::data(const QModelIndex &index, int role) const
{
    logText(LVL_DEBUG, LVL3(), QString("%1  index: [%2]  role: %3").arg(__FUNCTION__).arg(toString(index)).arg(role));

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
                if(index.column() == 0) {
                    result = item->icon();
                }
                break;
            case Qt::ForegroundRole:
            {
                TableHeader header = columnHeader(index.column());
                if(header.columnTextColor().isValid()) {
                    result = header.columnTextColor();
                }
                break;
            }
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
        result = orientation == Qt::Horizontal ? _columnHeaders.value(section).text() : _rowHeaders.value(section).text();
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
    else if(_rootItems.count() >= row + count) {
        QList<AbstractModelItem*> deleteItems = _rootItems.mid(row, count);
        beginRemoveRows(parentIndex, row, (row + count) - 1);
        _rootItems.remove(row, count);
        endRemoveRows();
        qDeleteAll(deleteItems);
        result = true;
    }
    return result;
}

bool AbstractItemModel::hasChildren(const QModelIndex& parent) const
{
    bool result = false;
    logText(LVL_DEBUG, LVL3(), QString("%1  index: [%2]").arg(__FUNCTION__).arg(toString(parent)));

    if(parent.isValid() && parent.internalPointer() != nullptr) {
        AbstractModelItem* parentItem = static_cast<AbstractModelItem*>(parent.internalPointer());
        result = parentItem->childrenRef().count() > 0;
    }
    else if(parent.isValid() == false) {
        result = _rootItems.count() > 0;
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

QModelIndex AbstractItemModel::firstIndexOfChildEntityUuid(const QModelIndex& parent, const QUuid& uuid) const
{
    QModelIndexList matches = match(parent, KANOOP::UUidRole, uuid, -1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
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

QModelIndexList AbstractItemModel::childIndexes(const QModelIndex& parent, int type) const
{
    QModelIndexList result;

    AbstractModelItem* item = static_cast<AbstractModelItem*>(parent.internalPointer());
    for(int row = 0;row < item->childCount();row++) {
        AbstractModelItem* child = item->child(row);
        QModelIndex index = createIndex(row, 0, child);
        if(child->entityType() == type) {
            result.append(index);
        }
        QModelIndexList indexes = childIndexes(index, type);
        result.append(indexes);
    }

    return result;
}

TableHeader::List AbstractItemModel::columnHeaders() const
{
    TableHeader::List result;
    QList<int> keys = _columnHeaders.keys();
    for(int key : keys) {
        result.append(_columnHeaders.value(key));
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

void AbstractItemModel::appendRootItems(QList<AbstractModelItem*> items)
{
    if(items.count() == 0) {
        return;
    }

    int firstRow = rowCount(QModelIndex());
    int lastRow = firstRow + items.count() - 1;
    QModelIndex insertIndex = index(firstRow, lastRow, QModelIndex());

    beginInsertRows(insertIndex, firstRow, lastRow);
    _rootItems.append(items);
    endInsertRows();
}

void AbstractItemModel::appendColumnHeader(int type, const QString &text)
{
    QString headerText = text.isEmpty() ? TableHeader::typeToString(type) : text;

    TableHeader header(type, headerText, Qt::Horizontal);
    _columnHeaders.insert(_columnHeaders.count(), header);
}

void AbstractItemModel::appendColumnHeader(int type, const QColor &columnTextColor, const QString &text)
{
    appendColumnHeader(type, text);
    _columnHeaders.setTextColorForType(type, columnTextColor);
}

void AbstractItemModel::insertColumnHeader(int type, int index, const QString& text)
{
    TableHeader::IntMap headers = _columnHeaders;
    _columnHeaders.clear();
    for(int col = 0;col < headers.count();col++) {
        TableHeader header = columnHeader(col);
        if(col != index) {
            appendColumnHeader(header.type(), header.columnTextColor(), header.text());
        }
        else {
            appendColumnHeader(type, text);
        }
    }
}

void AbstractItemModel::appendRowHeader(int type, const QString &value)
{
    QString text = value.isEmpty() ? TableHeader::typeToString(type) : value;

    TableHeader header(type, text, Qt::Vertical);
    _rowHeaders.insert(_columnHeaders.count(), header);
}

void AbstractItemModel::setColumnHeaderEntityMetadata(int type, const EntityMetadata& metadata)
{
    _columnHeaders.setEntityMetadataForType(type, metadata);
}

EntityMetadata AbstractItemModel::columnEntityMetadata(int type) const
{
    return _columnHeaders[type].entityMetadata();
}

void AbstractItemModel::setColumnHeaderVisible(int type, bool visible)
{
    _columnHeaders.setHeaderVisible(type, visible);
}

int AbstractItemModel::columnForHeader(int type) const
{
    int result = -1;
    for(int col = 0;col < _columnHeaders.count();col++) {
        if(_columnHeaders.value(col).type() == type) {
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
        int row = _rootItems.indexOf(item);
        QModelIndex index = createIndex(row, 0, item);
        beginRemoveRows(index.parent(), row, row);
        _rootItems.removeAll(item);
        endRemoveRows();
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

void AbstractItemModel::refreshAll()
{
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(rowCount() - 1, columnCount() - 1);
    emit dataChanged(topLeft, bottomRight);
}

void AbstractItemModel::emitRowChanged(const QModelIndex &rowIndex)
{
    QModelIndex firstColIndex = index(rowIndex.row(), 0, rowIndex.parent());
    QModelIndex lastColIndex = index(rowIndex.row(), columnCount(rowIndex) - 1, rowIndex.parent());
    emit dataChanged(firstColIndex, lastColIndex);
}

QString AbstractItemModel::toString(const QModelIndex &index)
{
    return QString("row: %1  col: %2  ptr: 0x%3")
            .arg(index.row())
            .arg(index.column())
            .arg((quint64)index.constInternalPointer(), 0, 16);
}



