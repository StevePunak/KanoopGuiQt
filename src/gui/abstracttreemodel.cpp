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

void AbstractTreeModel::columnChangedAtRowIndex(const QModelIndex &rowIndex, int columnHeader)
{
    int column = columnForHeader(columnHeader);
    if(column >= 0) {
        QModelIndex columntIndex = index(rowIndex.row(), column, rowIndex.parent());
        emit dataChanged(columntIndex, columntIndex);
    }
}
