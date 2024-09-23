/******************************************************************************************
**
** treeviewbase.cpp
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
** Created: Sun Oct 22 17:41:53 2023
**
******************************************************************************************/
#include <abstractitemmodel.h>
#include <guisettings.h>
#include <treeviewbase.h>

#include <Kanoop/stringutil.h>
#include <QHeaderView>
#include <QStyledItemDelegate>

TreeViewBase::TreeViewBase(QWidget *parent) :
    QTreeView(parent)
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

AbstractItemModel *TreeViewBase::sourceModel() const
{
    return static_cast<AbstractItemModel*>(model());
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

EntityMetadata TreeViewBase::findCurrentParent(int entityMetadataType) const
{
    EntityMetadata result;
    QModelIndex index = currentIndex().parent();
    while(index.isValid()) {
        EntityMetadata metadata;
        if((metadata = index.data(KANOOP::EntityMetadataRole).value<EntityMetadata>()).isValid() &&
            metadata.type() == entityMetadataType) {
            result = metadata;
            break;
        }
        index = index.parent();
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

