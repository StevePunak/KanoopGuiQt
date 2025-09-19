#include "abstractlistmodel.h"
#include <Kanoop/log.h>

AbstractListModel::AbstractListModel(QObject *parent) :
    AbstractItemModel(parent)
{
}

AbstractListModel::AbstractListModel(const QString &loggingCategory, QObject *parent) :
    AbstractItemModel(loggingCategory, parent)
{
}

void AbstractListModel::deleteRowAtIndex(const QModelIndex &index)
{
    AbstractModelItem* item = static_cast<AbstractModelItem*>(index.internalPointer());
    if(rootItems().contains(item)) {
        deleteRootItem(item);
    }
}

QVariant AbstractListModel::data(const QModelIndex &index, int role) const
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

