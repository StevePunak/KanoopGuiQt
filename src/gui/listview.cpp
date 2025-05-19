#include "listview.h"

#include <QSortFilterProxyModel>
#include <abstractitemmodel.h>

ListView::ListView(QWidget* parent) :
    QListView(parent),
    LoggingBaseClass()
{
}

void ListView::setModel(QAbstractItemModel* model)
{
    QSortFilterProxyModel* proxyModel = dynamic_cast<QSortFilterProxyModel*>(model);
    if(proxyModel == nullptr) {
        QListView::setModel(model);
        AbstractItemModel* abstractModel = dynamic_cast<AbstractItemModel*>(model);
        if(abstractModel != nullptr) {
            _sourceModel = abstractModel;
        }
        else {
            logText(LVL_ERROR, "The provided model is not an AbstractItemModel. Results are now undefined.");
        }
    }
    else {
        _proxyModel = proxyModel;
        _sourceModel = static_cast<AbstractItemModel*>(_proxyModel->sourceModel());
        QListView::setModel(_proxyModel);
    }

    connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &ListView::currentSelectionChanged);
}
