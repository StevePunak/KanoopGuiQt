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

    connect(selectionModel(), &QItemSelectionModel::currentChanged, this, &ListView::onCurrentSelectionChanged);
}

QModelIndex ListView::firstIndexOfEntityUuid(const QUuid& uuid) const
{
    QModelIndex result;
    if(model() == nullptr) {
        return result;
    }

    QModelIndexList indexes = model()->match(model()->index(0, 0, QModelIndex()), KANOOP::UUidRole, uuid, 1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
    if(indexes.count() > 0) {
        result = indexes.first();
    }
    return result;
}

QModelIndexList ListView::indexesOfUuid(const QUuid& uuid) const
{
    QModelIndexList result;
    if(model() != nullptr) {
        result = model()->match(model()->index(0, 0, QModelIndex()), KANOOP::UUidRole, uuid, -1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
        return result;
    }
    return result;
}

void ListView::setCurrentUuid(const QUuid& uuid, ScrollHint scrollHint)
{
    if(model() == nullptr) {
        return;
    }

    QModelIndexList indexes = sourceModel()->match(sourceModel()->index(0, 0, QModelIndex()), KANOOP::UUidRole, uuid, -1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
    for(const QModelIndex& index : indexes) {
        QModelIndex itemIndex = proxyModel() != nullptr ? proxyModel()->mapFromSource(index) : index;
        scrollTo(itemIndex, scrollHint);
        QItemSelection selection(itemIndex, itemIndex);
        selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current | QItemSelectionModel::Rows);
        selectionModel()->setCurrentIndex(itemIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current | QItemSelectionModel::Rows);
    }
}

void ListView::onCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
    Q_UNUSED(previous)
    emit currentIndexChanged(current);
    emit currentSelectionChanged();
}
