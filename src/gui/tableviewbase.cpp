/******************************************************************************************
**
** tableviewbase.cpp
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#include "abstractitemmodel.h"
#include "abstracttablemodel.h"
#include "columnsettingsdialog.h"
#include "guisettings.h"
#include "tableviewbase.h"
#include <QHeaderView>
#include <QMenu>
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
    connect(horizontalHeader(), &QHeaderView::customContextMenuRequested, this, &TableViewBase::onHeaderContextMenuRequested);
    connect(horizontalHeader(), &QHeaderView::sectionResized, this, &TableViewBase::onHorizontalHeaderResized);
    connect(verticalHeader(), &QHeaderView::sectionResized, this, &TableViewBase::onVerticalHeaderResized);

    // Enable context menu on header
    horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    // Create header actions
    _actionColSettings = new QAction("Column Settings", this);
    _actionHideCol = new QAction("Hide Column", this);
    _actionAutoResizeCols = new QAction("Auto-Resize Columns", this);
    _actionResetCols = new QAction("Reset Columns", this);
    connect(_actionColSettings, &QAction::triggered, this, &TableViewBase::onColumnSettingsClicked);
    connect(_actionHideCol, &QAction::triggered, this, &TableViewBase::onHideColumnClicked);
    connect(_actionAutoResizeCols, &QAction::triggered, this, &TableViewBase::onAutoResizeColumnsClicked);
    connect(_actionResetCols, &QAction::triggered, this, &TableViewBase::onResetColumnsClicked);

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

int TableViewBase::rowCount() const
{
    if(sourceModel() != nullptr) {
        return sourceModel()->rowCount();
    }
    return 0;
}

QModelIndexList TableViewBase::allRows() const
{
    QModelIndexList result;
    if(model() == nullptr) {
        return result;
    }

    for(int row = 0;row < rowCount();row++) {
        QModelIndex rowIndex = model()->index(row, 0);
        result.append(rowIndex);
    }
    return result;
}

QModelIndex TableViewBase::findFirstMatch(const QVariant& needle, int role) const
{
    QModelIndex result;
    if(sourceModel() != nullptr) {
        QModelIndexList matches = model()->match(model()->index(0, 0, QModelIndex()), role, needle, -1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
        if(matches.count() > 0) {
            result = matches.at(0);
        }
    }
    return result;
}

void TableViewBase::setCurrentUuid(const QUuid& uuid, ScrollHint scrollHint)
{
    if(model() == nullptr) {
        return;
    }

    QModelIndexList indexes = model()->match(model()->index(0, 0, QModelIndex()), KANOOP::UUidRole, uuid, -1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
    for(const QModelIndex& index : indexes) {
        scrollTo(index, scrollHint);
        selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current | QItemSelectionModel::Rows);
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

void TableViewBase::clear()
{
    if(_sourceModel != nullptr) {
        _sourceModel->clear();
    }
}

void TableViewBase::currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    emit currentIndexChanged(current, previous);
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

void TableViewBase::onHeaderContextMenuRequested()
{
    QMenu menu;
    menu.addAction(_actionColSettings);
    menu.addAction(_actionHideCol);
    menu.addAction(_actionAutoResizeCols);
    menu.addSeparator();
    menu.addAction(_actionResetCols);

    menu.exec(QCursor::pos());
}

void TableViewBase::onColumnSettingsClicked()
{
    if(sourceModel() == nullptr) {
        return;
    }

    ColumnSettingsDialog dlg(sourceModel()->columnHeaders(), parentWidget());
    if(dlg.exec() == QDialog::Accepted) {
        for(const TableHeader& header : dlg.headers()) {
            sourceModel()->setColumnHeaderVisible(header.type(), header.isVisible());
            int column = sourceModel()->columnForHeader(header.type());
            setColumnHidden(column, header.isVisible() == false);
        }
        GuiSettings::globalInstance()->saveLastHeaderState(horizontalHeader(), sourceModel());
    }
}

void TableViewBase::onHideColumnClicked()
{
    int section = horizontalHeader()->logicalIndexAt(QCursor::pos());
    if(section < 0 || sourceModel() == nullptr) {
        return;
    }
    TableHeader header = sourceModel()->columnHeader(section);
    if(header.isValid() == false) {
        return;
    }

    sourceModel()->setColumnHeaderVisible(header.type(), false);
    setColumnHidden(section, true);
    GuiSettings::globalInstance()->saveLastHeaderState(horizontalHeader(), sourceModel());
}

void TableViewBase::onAutoResizeColumnsClicked()
{
    horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void TableViewBase::onResetColumnsClicked()
{
    for(int section = 0;section < horizontalHeader()->count();section++) {
        horizontalHeader()->resizeSection(section, 120);
    }
    GuiSettings::globalInstance()->saveLastHeaderState(horizontalHeader(), sourceModel());
}

