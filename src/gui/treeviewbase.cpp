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
#include "columnsettingsdialog.h"

#include <QMenu>
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
    connect(header(), &QHeaderView::customContextMenuRequested, this, &TreeViewBase::onHeaderContextMenuRequested);

    // Enable context menu on header
    header()->setContextMenuPolicy(Qt::CustomContextMenu);

    // Create header actions
    _actionColSettings = new QAction("Column Settings", this);
    _actionHideCol = new QAction("Hide Column", this);
    _actionAutoResizeCols = new QAction("Auto-Resize Columns", this);
    _actionResetCols = new QAction("Reset Columns", this);
    connect(_actionColSettings, &QAction::triggered, this, &TreeViewBase::onColumnSettingsClicked);
    connect(_actionHideCol, &QAction::triggered, this, &TreeViewBase::onHideColumnClicked);
    connect(_actionAutoResizeCols, &QAction::triggered, this, &TreeViewBase::onAutoResizeColumnsClicked);
    connect(_actionResetCols, &QAction::triggered, this, &TreeViewBase::onResetColumnsClicked);
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
    if(model() == nullptr) {
        return result;
    }

    QModelIndexList indexes = model()->match(model()->index(0, 0, QModelIndex()), KANOOP::UUidRole, uuid, 1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
    if(indexes.count() > 0) {
        result = indexes.first();
    }
    return result;
}

void TreeViewBase::setCurrentUuid(const QUuid &uuid, ScrollHint scrollHint)
{
    if(model() == nullptr) {
        return;
    }

    QModelIndexList indexes = model()->match(model()->index(0, 0, QModelIndex()), KANOOP::UUidRole, uuid, -1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
    for(const QModelIndex& index : indexes) {
        QModelIndexList parents = findParents(index);
        for(const QModelIndex& parentIndex : parents) {
            setExpanded(parentIndex, true);
        }
        scrollTo(index, scrollHint);
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
    connect(selectionModel(), &QItemSelectionModel::currentChanged, this, &TreeViewBase::onCurrentSelectionChanged);
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

bool TreeViewBase::isLeafExpanded(const QModelIndex& index, bool recursive) const
{
    if(sourceModel() == nullptr) {
        return false;
    }

    bool result = isExpanded(index);
    if(result == false) {
        return false;
    }

    if(recursive == false) {
        return result;
    }

    int rowCount = model()->rowCount(index);
    for(int row = 0;row < rowCount;row++) {
        QModelIndex childIndex = model()->index(row, 0, index);
        if (childIndex.isValid()) {
            if(isLeafExpanded(childIndex, recursive) == false) {
                return false;
            }
        }
    }
    return true;
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

int TreeViewBase::columnForHeaderType(int headerType) const
{
    if(sourceModel() != nullptr) {
        return sourceModel()->columnForHeader(headerType);
    }
    return -1;
}

void TreeViewBase::setColumnTypesVisible(const QList<int>& headerTypes, bool visible, bool exclusive)
{
    QList<int> columns;
    for(int headerType : headerTypes) {
        int col = columnForHeaderType(headerType);
        if(col >= 0) {
            columns.append(col);
        }
    }
    setColumnsVisible(columns, visible, exclusive);
}

void TreeViewBase::setColumnsVisible(const QList<int>& columns, bool visible, bool exclusive)
{
    for(int col = 0;col < header()->count();col++) {
        if(columns.contains(col)) {
            setColumnHidden(col, visible == false);
        }
        else if(exclusive) {
            setColumnHidden(col, visible);
        }
    }
}

void TreeViewBase::refreshVisibleColumns(const QList<int>& columns)
{
    QRect visibleRect = viewport()->rect();

    QModelIndexList update;

    // first get all the column widths
    QMap<int, int> colWidths;
    QMap<int, int> colXValues;
    QList<int> visibleColumns;
    int x = visibleRect.left();
    while(x < visibleRect.right()) {
        int col = columnAt(x);
        if(col < 0) {
            break;
        }
        int width = columnWidth(col);
        colWidths.insert(col, width);
        colXValues.insert(col, x);
        visibleColumns.append(col);
        x += width;
    }

    // now get row heights in the visible rect
    QList<int> rowHeights;
    for(int y = 0, x = 0;y < visibleRect.height();) {
        QPoint point(x, y);
        QModelIndex index = indexAt(point);
        if(index.isValid()) {
            int height = rowHeight(index);
            rowHeights.append(height);
            y += height;
        }
        else {
            break;
        }
    }

    for(int row = 0, y = 0;row < rowHeights.count();row++) {
        for(int visibleColumnIndex = 0, x = 0;visibleColumnIndex < colWidths.count();visibleColumnIndex++) {
            x = colXValues[visibleColumnIndex];
            int col = visibleColumns[visibleColumnIndex];
            QModelIndex index = indexAt(QPoint(x, y));
            if(proxyModel() != nullptr) {
                index = proxyModel()->mapToSource(index);
            }
            if(columns.contains(col) && index.isValid() && update.contains(index) == false) {
                QVariant data = index.data(Qt::DisplayRole);
                if(data.isValid()) {
                    update.append(index);
                }
            }
        }
        y += rowHeights.at(row);
    }

    for(const QModelIndex& index : update) {
        refreshIndex(index);
    }
}

/**
 * NOTE: This can get very expensive in a large model.
 *       Prefer refreshVisibleColumns()
 */
void TreeViewBase::refreshVisibleIndexes(const QModelIndexList& indexes)
{
    QRect visibleRect = viewport()->rect();
    QModelIndexList update;
    for(const QModelIndex& index : indexes) {
        if(proxyModel() != nullptr) {
            QModelIndex proxyIndex = proxyModel()->mapFromSource(index);
            QRect rect = visualRect(proxyIndex);
            if(rect.isValid() && visibleRect.intersects(rect)) {
                update.append(proxyModel()->mapToSource(proxyIndex));
            }
        }
        else {
            QRect rect = visualRect(index);
            if(rect.isValid() && visibleRect.intersects(rect)) {
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

void TreeViewBase::onHeaderContextMenuRequested()
{
    _contextMenuPos = mapFromGlobal(QCursor::pos());

    QMenu menu;
    menu.addAction(_actionColSettings);
    menu.addAction(_actionHideCol);
    menu.addAction(_actionAutoResizeCols);
    menu.addSeparator();
    menu.addAction(_actionResetCols);

    menu.exec(QCursor::pos());
}

void TreeViewBase::onColumnSettingsClicked()
{
    if(sourceModel() == nullptr) {
        return;
    }

    ColumnSettingsDialog dlg(sourceModel()->columnHeaders(), parentWidget());
    if(dlg.exec() == QDialog::Accepted) {
        for(const TableHeader& columnHeader : dlg.headers()) {
            sourceModel()->setColumnHeaderVisible(columnHeader.type(), columnHeader.isVisible());
            int column = sourceModel()->columnForHeader(columnHeader.type());
            setColumnHidden(column, columnHeader.isVisible() == false);
        }
        GuiSettings::globalInstance()->saveLastHeaderState(header(), sourceModel());
    }
}

void TreeViewBase::onHideColumnClicked()
{
    int section = header()->logicalIndexAt(_contextMenuPos);
    if(section < 0 || sourceModel() == nullptr) {
        return;
    }
    TableHeader columnHeader = sourceModel()->columnHeader(section);
    if(columnHeader.isValid() == false) {
        return;
    }

    sourceModel()->setColumnHeaderVisible(columnHeader.type(), false);
    setColumnHidden(section, true);
    GuiSettings::globalInstance()->saveLastHeaderState(header(), sourceModel());
}

void TreeViewBase::onAutoResizeColumnsClicked()
{
    header()->resizeSections(QHeaderView::ResizeToContents);
}

void TreeViewBase::onResetColumnsClicked()
{
    for(int section = 0;section < header()->count();section++) {
        header()->resizeSection(section, 120);
    }
    GuiSettings::globalInstance()->saveLastHeaderState(header(), sourceModel());
}

void TreeViewBase::onCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
    Q_UNUSED(previous)
    emit currentSelectionChanged();
    if(current.isValid()) {
        emit currentIndexChanged(current);
    }
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

