/******************************************************************************************
**
** treeviewbase.h
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Sun Oct 22 17:41:53 2023
**
******************************************************************************************/
#ifndef TREEVIEWBASE_H
#define TREEVIEWBASE_H
#include <Kanoop/entitymetadata.h>

#include <QTreeView>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class QSortFilterProxyModel;
class QStyledItemDelegate;
class AbstractItemModel;
class LIBKANOOPGUI_EXPORT TreeViewBase : public QTreeView,
                                         public LoggingBaseClass
{
    Q_OBJECT
public:
    TreeViewBase(QWidget* parent = nullptr);

    int entityTypeAtPos(const QPoint& pos);
    EntityMetadata currentMetadata() const;
    EntityMetadata metadataAtPos(const QPoint& pos) const;

    QModelIndex firstIndexOfEntityUuid(const QUuid& uuid) const;
    QModelIndexList indexesOfUuid(const QUuid& uuid) const;
    void setCurrentUuid(const QUuid& uuid);

    QByteArray saveState() const;
    void restoreState(const QByteArray& state);

    void restoreHeaderStates();

    virtual void setModel(QAbstractItemModel* model) override;
    AbstractItemModel* sourceModel() const { return _sourceModel; }
    QSortFilterProxyModel* proxyModel() const { return _proxyModel; }

    void collapseRecursively(const QModelIndex& index, int depth = -1);

    void setColumnDelegate(int type, QStyledItemDelegate* delegate);

public slots:
    virtual void refreshVisibleColumns(const QList<int>& columns);
    virtual void refreshVisibleIndexes(const QModelIndexList& indexes);
    virtual void refreshIndex(const QModelIndex& sourceIndex);
    virtual void clear();

protected:
    EntityMetadata findCurrentParent(int entityMetadataType) const;
    EntityMetadata findFirstParent(const QModelIndex& index, int entityMetadataType) const;
    QModelIndexList findParents(const QModelIndex& index) const;

private:
    AbstractItemModel* _sourceModel;
    QSortFilterProxyModel* _proxyModel;
    QMap<int, QStyledItemDelegate*> _columnDelegates;

    QAction* _actionColSettings = nullptr;
    QAction* _actionHideCol = nullptr;
    QAction* _actionAutoResizeCols = nullptr;
    QAction* _actionResetCols = nullptr;

signals:
    void itemProgramaticallySelected(const QModelIndex& index);
    void headerChanged();
    void currentSelectionChanged();

private slots:
    virtual void onHorizontalHeaderResized(int /*logicalIndex*/, int /*oldSize*/, int /*newSize*/);

    void onHeaderContextMenuRequested();
    void onColumnSettingsClicked();
    void onHideColumnClicked();
    void onAutoResizeColumnsClicked();
    void onResetColumnsClicked();
};

#endif // TREEVIEWBASE_H
