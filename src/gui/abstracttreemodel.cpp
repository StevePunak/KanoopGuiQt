/******************************************************************************************
**
** abstracttreemodel.cpp
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Tue Oct 10 09:37:57 2023
**
******************************************************************************************/
#include "abstracttreemodel.h"

AbstractTreeModel::AbstractTreeModel(QObject *parent) :
    AbstractItemModel(parent)
{
    AbstractItemModel::setObjectName(AbstractItemModel::metaObject()->className());
}

AbstractTreeModel::AbstractTreeModel(const QString &loggingCategory, QObject *parent) :
    AbstractItemModel(loggingCategory, parent)
{
    AbstractItemModel::setObjectName(AbstractItemModel::metaObject()->className());
}

int AbstractTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    int result = qMax(1, columnHeadersIntMap().count());
    return result;
}

QVariant AbstractTreeModel::data(const QModelIndex &index, int role) const
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

void AbstractTreeModel::columnChangedAtRowIndex(const QModelIndex &rowIndex, int columnHeader)
{
    int column = columnForHeader(columnHeader);
    if(column >= 0) {
        QModelIndex columntIndex = index(rowIndex.row(), column, rowIndex.parent());
        emit dataChanged(columntIndex, columntIndex);
    }
}
