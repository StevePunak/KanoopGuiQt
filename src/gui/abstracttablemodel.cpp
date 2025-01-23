/******************************************************************************************
**
** abstracttablemodel.cpp
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Wed Oct 25 14:21:04 2023
**
******************************************************************************************/
#include "abstracttablemodel.h"
#include <Kanoop/log.h>

AbstractTableModel::AbstractTableModel(QObject *parent) :
    AbstractItemModel(parent)
{
}

AbstractTableModel::AbstractTableModel(const QString &loggingCategory, QObject *parent) :
    AbstractItemModel(loggingCategory, parent)
{
}

void AbstractTableModel::deleteRowAtIndex(const QModelIndex &index)
{
    AbstractModelItem* item = static_cast<AbstractModelItem*>(index.internalPointer());
    if(rootItems().contains(item)) {
        beginRemoveRows(index, index.row(), 1);
        deleteRootItem(item);
        endRemoveRows();
    }
}

int AbstractTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return columnHeadersIntMap().count();
}

QVariant AbstractTableModel::data(const QModelIndex &index, int role) const
{
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
            default:
                break;
            }
        }
    }

    return result;
}

void AbstractTableModel::columnChangedAtRowIndex(const QModelIndex &rowIndex, int columnHeader)
{
    int column = columnForHeader(columnHeader);
    if(column >= 0) {
        QModelIndex columntIndex = index(rowIndex.row(), column, rowIndex.parent());
        emit dataChanged(columntIndex, columntIndex);
    }
}

