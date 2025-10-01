#include <treeselectionmodel.h>

#include <abstractitemmodel.h>

TreeSelectionModel::TreeSelectionModel(QAbstractItemModel* model, QObject* parent) :
    QItemSelectionModel(model, parent)
{
}

void TreeSelectionModel::select(const QModelIndex& index, SelectionFlags command)
{
    QModelIndex parentIndex = index.parent();
    QModelIndexList selectedIndexes = selection().indexes();

    if (selectedIndexes.isEmpty() || selectedIndexes.first().parent() == parentIndex) {
        // Allow selection if no items are selected, or if the new item shares the same parent.
        QItemSelectionModel::select(index, command);
    } else {
        // If the parent is different, clear the existing selection and select the new item.
        clearSelection();
        QItemSelectionModel::select(index, command);
    }
}

void TreeSelectionModel::select(const QItemSelection& selection, SelectionFlags command)
{
    if(selection.count() > 0) {
        QModelIndexList indexes = selection.indexes();
        QModelIndex parent = indexes.at(0).parent();
        bool haveSameParent = true;
        for(const QModelIndex& index : indexes) {
            if(index.parent() != parent) {
                haveSameParent = false;
                clearSelection();
            }
        }

        if(haveSameParent) {
            QItemSelectionModel::select(selection, command);
        }
    }
    else {
        QItemSelectionModel::select(selection, command);
    }
}
