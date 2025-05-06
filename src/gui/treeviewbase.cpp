/******************************************************************************************
**
** treeviewbase.cpp
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Sun Oct 22 17:41:53 2023
**
******************************************************************************************/
#include <QSortFilterProxyModel>
#include <abstractitemmodel.h>
#include <guisettings.h>
#include <treeviewbase.h>

#include <Kanoop/stringutil.h>
#include <QHeaderView>
#include <QStyledItemDelegate>

TreeViewBase::TreeViewBase(QWidget *parent) :
    QTreeView(parent),
    LoggingBaseClass(),
    _sourceModel(nullptr)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setExpandsOnDoubleClick(false);

    // Wire up signals for saving header state
    connect(header(), &QHeaderView::sectionResized, this, &TreeViewBase::onHorizontalHeaderResized);
}

int TreeViewBase::entityTypeAtPos(const QPoint &pos)
{
    int entityType = 0;
    QModelIndex index = indexAt(pos);
    if(index.isValid()) {
        QVariant value = index.data(KANOOP::EntityTypeRole);
        if(value.isValid()) {
            entityType = value.toInt();
        }
    }
    return entityType;
}

EntityMetadata TreeViewBase::currentMetadata() const
{
    EntityMetadata result;
    QModelIndex index = currentIndex();
    if(index.isValid()) {
        result = index.data(KANOOP::EntityMetadataRole).value<EntityMetadata>();
    }
    return result;
}

EntityMetadata TreeViewBase::metadataAtPos(const QPoint &pos) const
{
    EntityMetadata metadata;
    QModelIndex index = indexAt(pos);
    if(index.isValid()) {
        metadata = index.data(KANOOP::EntityMetadataRole).value<EntityMetadata>();
    }
    return metadata;
}

QModelIndex TreeViewBase::firstIndexOfEntityUuid(const QUuid& uuid) const
{
    QModelIndex result;
    QModelIndexList indexes = model()->match(model()->index(0, 0, QModelIndex()), KANOOP::UUidRole, uuid, 1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
    if(indexes.count() > 0) {
        result = indexes.first();
    }
    return result;
}

void TreeViewBase::setCurrentUuid(const QUuid &uuid)
{
    AbstractItemModel* itemModel = dynamic_cast<AbstractItemModel*>(model());
    if(itemModel == nullptr) {
        return;
    }

    QModelIndexList indexes = itemModel->indexesOfEntityUuid(uuid);
    for(const QModelIndex& index : indexes) {
        QModelIndexList parents = findParents(index);
        for(const QModelIndex& parentIndex : parents) {
            setExpanded(parentIndex, true);
        }
        scrollTo(index, EnsureVisible);
        selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current | QItemSelectionModel::Rows);
        expand(index);
        emit itemProgramaticallySelected(index);
    }
}

QByteArray TreeViewBase::saveState() const
{
    QByteArray result;
    AbstractItemModel* itemModel = dynamic_cast<AbstractItemModel*>(model());
    if(itemModel != nullptr) {
        QList<QUuid> uuids;
        QModelIndexList persistentIndexes = itemModel->getPersistentIndexes();
        for(const QModelIndex& index : persistentIndexes) {
            if(isExpanded(index)) {
                QUuid uuid = index.data(KANOOP::UUidRole).toUuid();
                if(uuid.isNull() == false) {
                    uuids.append(uuid);
                }
            }
        }

        result = StringUtil::toString(uuids).toUtf8();
    }
    return result;
}

void TreeViewBase::restoreState(const QByteArray &state)
{
    AbstractItemModel* itemModel = dynamic_cast<AbstractItemModel*>(model());
    if(itemModel == nullptr) {
        return;
    }
    QList<QUuid> uuids = StringUtil::uuidsFromString(QString(state));
    for(const QUuid& uuid : uuids) {
        QModelIndex index = itemModel->firstIndexOfEntityUuid(uuid);
        if(index.isValid()) {
            scrollTo(index, EnsureVisible);
            expand(index);
        }
    }
}

void TreeViewBase::restoreHeaderStates()
{
    if(GuiSettings::globalInstance() != nullptr && model() != nullptr) {
        AbstractItemModel* itemModel = static_cast<AbstractItemModel*>(sourceModel());
        GuiSettings::globalInstance()->restoreLastHeaderState(header(), itemModel);
    }
}

void TreeViewBase::setModel(QAbstractItemModel* model)
{
    QSortFilterProxyModel* proxyModel = dynamic_cast<QSortFilterProxyModel*>(model);
    if(proxyModel == nullptr) {
        QTreeView::setModel(model);
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
        QTreeView::setModel(_proxyModel);
    }
    connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &TreeViewBase::currentSelectionChanged);
}

void TreeViewBase::refreshVisible()
{
    QModelIndex topLeft = indexAt(rect().topLeft());
    QModelIndex bottomRight = indexAt(rect().bottomRight());
    _sourceModel->refresh(topLeft, bottomRight);
    update();
}

void TreeViewBase::collapseRecursively(const QModelIndex& index, int depth)
{
    collapse(index);
    int rows = model()->rowCount(index);
    if(depth != 0) {
        for(int row = 0;row < rows;row++) {
            QModelIndex child = model()->index(row, 0, index);
            collapseRecursively(child, depth < 0 ? -1 : depth - 1);
        }
    }
}

void TreeViewBase::setColumnDelegate(int type, QStyledItemDelegate *delegate)
{
    QStyledItemDelegate* existing = _columnDelegates.value(type);
    if(existing == nullptr) {
        _columnDelegates.remove(type);
        delete existing;
    }
    int column = sourceModel()->columnForHeader(type);
    if(column != -1) {
        _columnDelegates.insert(type, delegate);
        setItemDelegateForColumn(column, delegate);
    }
}

void TreeViewBase::refreshVisibleIndexes(const QModelIndexList& indexes)
{
    QRect visibleRect = viewport()->rect();
    QModelIndexList update;
    for(const QModelIndex& index : indexes) {
        if(proxyModel() != nullptr) {
            QModelIndex proxyIndex = proxyModel()->mapFromSource(index);
            QRect rect = visualRect(proxyIndex);
            if(rect.isValid() && visibleRect.contains(rect)) {
                update.append(proxyModel()->mapToSource(proxyIndex));
            }
        }
        else {
            QRect rect = visualRect(index);
            if(rect.isValid() && visibleRect.contains(rect)) {
                update.append(index);
            }
        }
    }

    for(const QModelIndex& index : update) {
        refreshIndex(index);
    }
}

void TreeViewBase::refreshIndex(const QModelIndex& sourceIndex)
{
    sourceModel()->refresh(sourceIndex, sourceIndex);
}

void TreeViewBase::clear()
{
    if(_sourceModel != nullptr) {
        _sourceModel->clear();
    }
}

EntityMetadata TreeViewBase::findCurrentParent(int entityMetadataType) const
{
    return findFirstParent(currentIndex(), entityMetadataType);
}

EntityMetadata TreeViewBase::findFirstParent(const QModelIndex& index, int entityMetadataType) const
{
    EntityMetadata result;
    QModelIndex parentIndex = index.parent();
    while(parentIndex.isValid()) {
        EntityMetadata metadata;
        if((metadata = parentIndex.data(KANOOP::EntityMetadataRole).value<EntityMetadata>()).isValid() &&
            metadata.type() == entityMetadataType) {
            result = metadata;
            break;
        }
        parentIndex = parentIndex.parent();
    }
    return result;
}

QModelIndexList TreeViewBase::findParents(const QModelIndex &index) const
{
    QModelIndexList result;
    QModelIndex currentIndex = index;
    while(currentIndex.isValid()) {
        result.append(currentIndex);
        currentIndex = currentIndex.parent();
    }
    return result;
}

void TreeViewBase::onHorizontalHeaderResized(int, int, int)
{
    if(GuiSettings::globalInstance() != nullptr && model() != nullptr) {
        AbstractItemModel* itemModel = static_cast<AbstractItemModel*>(sourceModel());
        GuiSettings::globalInstance()->saveLastHeaderState(header(), itemModel);
    }
    emit headerChanged();
}

QModelIndexList TreeViewBase::indexesOfUuid(const QUuid& uuid) const
{
    QModelIndexList result;
    if(model() != nullptr) {
        result = model()->match(model()->index(0, 0, QModelIndex()), KANOOP::UUidRole, uuid, -1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
        return result;
    }
    return result;
}

