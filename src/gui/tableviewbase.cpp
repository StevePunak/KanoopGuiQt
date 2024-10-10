/******************************************************************************************
**
** tableviewbase.cpp
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
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#include "abstractitemmodel.h"
#include "abstracttablemodel.h"
#include "guisettings.h"
#include "tableviewbase.h"
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>

TableViewBase::TableViewBase(QWidget *parent) :
    QTableView(parent),
    _sourceModel(nullptr), _proxyModel(nullptr)
{
    // No grid
    setGridStyle(Qt::NoPen);

    // Select whole rows
    setSelectionBehavior(QAbstractItemView::SelectRows);

    // Align Text Left
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    verticalHeader()->setDefaultAlignment(Qt::AlignLeft);

    // Vertical sizing
    verticalHeader()->setDefaultSectionSize(1);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    // Stretch last column
    horizontalHeader()->setStretchLastSection(true);

    // Wire up signals for saving header state
    connect(horizontalHeader(), &QHeaderView::sectionResized, this, &TableViewBase::onHorizontalHeaderResized);
    connect(verticalHeader(), &QHeaderView::sectionResized, this, &TableViewBase::onVerticalHeaderResized);

    // Custom context menu is the default
    setContextMenuPolicy(Qt::CustomContextMenu);
}

TableViewBase::~TableViewBase()
{
    qDeleteAll(_columnDelegates);
}

void TableViewBase::setModel(QAbstractItemModel *model)
{
    QSortFilterProxyModel* proxyModel = dynamic_cast<QSortFilterProxyModel*>(model);
    if(proxyModel == nullptr) {
        QTableView::setModel(model);
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
        QTableView::setModel(_proxyModel);
    }

    connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &TableViewBase::currentSelectionChanged);
}

int TableViewBase::entityTypeAtPos(const QPoint &pos)
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

EntityMetadata TableViewBase::currentMetadata() const
{
    EntityMetadata result;
    QModelIndex index = currentIndex();
    EntityMetadata metadata;
    if(index.isValid()) {
        metadata = index.data(KANOOP::EntityMetadataRole).value<EntityMetadata>();
        result = metadata;
    }
    return result;
}

EntityMetadata TableViewBase::metadataAtPos(const QPoint &pos) const
{
    EntityMetadata metadata;
    QModelIndex index = indexAt(pos);
    if(index.isValid()) {
        metadata = index.data(KANOOP::EntityMetadataRole).value<EntityMetadata>();
    }
    return metadata;
}

void TableViewBase::deleteRow(const QModelIndex &index)
{
    AbstractTableModel* tableModel = dynamic_cast<AbstractTableModel*>(sourceModel());
    if(index.isValid() && tableModel != nullptr) {
        QModelIndex sourceIndex = proxyModel()->mapToSource(index);
        if(sourceIndex.isValid()) {
            tableModel->deleteRowAtIndex(sourceIndex);
        }
    }
}

void TableViewBase::restoreHeaderStates()
{
    restoreHorizontalHeaderState();
    restoreVerticalHeaderState();
}

void TableViewBase::restoreHorizontalHeaderState()
{
    if(GuiSettings::globalInstance() != nullptr && model() != nullptr) {
        AbstractItemModel* itemModel = static_cast<AbstractItemModel*>(sourceModel());
        GuiSettings::globalInstance()->restoreLastHeaderState(horizontalHeader(), itemModel);
    }
}

void TableViewBase::restoreVerticalHeaderState()
{
    if(GuiSettings::globalInstance() != nullptr && model() != nullptr) {
        AbstractItemModel* itemModel = static_cast<AbstractItemModel*>(sourceModel());
        GuiSettings::globalInstance()->restoreLastHeaderState(verticalHeader(), itemModel);
    }
}

void TableViewBase::setColumnDelegate(int type, QStyledItemDelegate *delegate)
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

void TableViewBase::onHorizontalHeaderResized(int, int, int)
{
    if(GuiSettings::globalInstance() != nullptr && model() != nullptr) {
        AbstractItemModel* itemModel = static_cast<AbstractItemModel*>(sourceModel());
        GuiSettings::globalInstance()->saveLastHeaderState(horizontalHeader(), itemModel);
    }
    emit horizontalHeaderChanged();
}

void TableViewBase::onVerticalHeaderResized(int, int, int)
{
    if(GuiSettings::globalInstance() != nullptr && model() != nullptr) {
        AbstractItemModel* itemModel = static_cast<AbstractItemModel*>(sourceModel());
        GuiSettings::globalInstance()->saveLastHeaderState(verticalHeader(), itemModel);
    }
    emit verticalHeaderChanged();
}

