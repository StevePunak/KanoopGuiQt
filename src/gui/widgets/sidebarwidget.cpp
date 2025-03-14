#include "widgets/sidebarwidget.h"
#include "sidebarwidgetprivate.h"
#include "guitypes.h"
#include <Kanoop/kanoopcommon.h>

#include <QStandardItemModel>
#include <resources.h>

const QSize SidebarWidget::DefaultIconSize              = QSize(32, 32);
const QMargins SidebarWidget::DefaultContentsMargins    = QMargins(4, 4, 4, 4);
const int SidebarWidget::DefaultVerticalSpacing         = 5;
const int SidebarWidget::DefaultHorizontalSpacing       = 5;

SidebarWidget::SidebarWidget(QWidget *parent) :
    QListView{parent}
{
    _delegate = new SidebarPaintDelegate(this);

    connect(this, &SidebarWidget::clicked, this, &SidebarWidget::onItemClicked);

    setVerticalSpacing(DefaultVerticalSpacing);
    setHorizontalSpacing(DefaultHorizontalSpacing);
    setContentsMargins(DefaultContentsMargins);
    setIconSize(DefaultIconSize);

    setModel(new QStandardItemModel(this));
    setItemDelegate(_delegate);
}

void SidebarWidget::addItem(int entityMetadataType, const QString &text, int imageResourceId)
{
    QIcon icon;
    if(imageResourceId != 0) {
        icon = Resources::getIcon(imageResourceId);
    }
    SidebarItem* item = new SidebarItem(entityMetadataType, text, icon);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setData(entityMetadataType, KANOOP::EntityTypeRole);
    static_cast<QStandardItemModel*>(model())->appendRow(item);
}

void SidebarWidget::removeItem(int entityMetadataType)
{
    QStandardItemModel* model = static_cast<QStandardItemModel*>(SidebarWidget::model());
    QModelIndexList matches = model->match(model->index(0, 0, QModelIndex()), KANOOP::EntityTypeRole, entityMetadataType, -1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
    while(matches.count() > 0) {
        QModelIndex index = matches.takeFirst();
        model->removeRow(index.row());
    }
}

bool SidebarWidget::containsItem(int entityMetadataType) const
{
    QStandardItemModel* model = static_cast<QStandardItemModel*>(SidebarWidget::model());
    QModelIndexList matches = model->match(model->index(0, 0, QModelIndex()), KANOOP::EntityTypeRole, entityMetadataType, -1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
    return matches.count() > 0;
}

void SidebarWidget::clear()
{
    static_cast<QStandardItemModel *>(model())->clear();
}

void SidebarWidget::selectItem(int entityMetadataType)
{
    QModelIndex index;
    QModelIndex startSearchIndex = sourceModel()->index(0, 0, QModelIndex());
    QModelIndexList found = sourceModel()->match(startSearchIndex, KANOOP::EntityTypeRole, entityMetadataType, 1, Qt::MatchRecursive | Qt::MatchWrap);
    if(found.count() > 0) {
        index = found.first();
        selectionModel()->select(index, QItemSelectionModel::SelectCurrent);
    }
}

QMargins SidebarWidget::contentsMargins() const
{
    return _delegate->contentsMargins();
}

void SidebarWidget::setContentsMargins(const QMargins &value)
{
    _delegate->setContentsMargins(value);
}

int SidebarWidget::horizontalSpacing() const
{
    return _delegate->horizontalSpacing();
}

void SidebarWidget::setHorizontalSpacing(int value)
{
    _delegate->setHorizontalSpacing(value);
}

int SidebarWidget::verticalSpacing() const
{
    return _delegate->verticalSpacing();
}

void SidebarWidget::setVerticalSpacing(int value)
{
    _delegate->setVerticalSpacing(value);
}

QSize SidebarWidget::iconSize() const
{
    return _delegate->iconSize();
}

void SidebarWidget::setIconSize(const QSize &value)
{
    _delegate->setIconSize(value);
}

QStandardItemModel* SidebarWidget::sourceModel() const
{
    return static_cast<QStandardItemModel *>(model());
}

void SidebarWidget::onItemClicked(const QModelIndex &index)
{
    int itemType = index.data(KANOOP::EntityTypeRole).toInt();
    if(itemType != 0) {
        emit itemClicked(itemType);
    }
}
