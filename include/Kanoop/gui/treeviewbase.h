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

    virtual EntityMetadata currentMetadata() const;
    virtual EntityMetadata metadataAtPos(const QPoint& pos) const;

    virtual QModelIndex firstIndexOfEntityUuid(const QUuid& uuid) const;
    virtual QModelIndexList indexesOfUuid(const QUuid& uuid) const;
    virtual void setCurrentUuid(const QUuid& uuid, ScrollHint scrollHint = EnsureVisible);
    virtual void setCurrentIndex(const QModelIndex& index, ScrollHint scrollHint = EnsureVisible);

    virtual QModelIndex findNextMatch(const QString& text, const QModelIndex& fromIndex) const;
    virtual QModelIndex findPreviousMatch(const QString& text, const QModelIndex& fromIndex) const;
    virtual QModelIndex finalChildIndex(const QModelIndex& from) const;
    virtual QModelIndex nextIndex(const QModelIndex& from) const;
    virtual QModelIndex previousIndex(const QModelIndex& from) const;
    virtual QModelIndexList matchBackwards(const QModelIndex &start, int role,
                                           const QVariant &value, int hits = 1,
                                           Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const;

    QModelIndex currentSourceIndex() const;

    QByteArray saveState() const;
    void restoreState(const QByteArray& state);

    void restoreHeaderStates();

    virtual void setModel(QAbstractItemModel* model) override;
    virtual void setSelectionModel(QItemSelectionModel* selectionModel) override;
    AbstractItemModel* sourceModel() const { return _sourceModel; }
    QSortFilterProxyModel* proxyModel() const { return _proxyModel; }

    void collapseRecursively(const QModelIndex& index, int depth = -1);
    bool isLeafExpanded(const QModelIndex& index, bool recursive = true) const;

    void setColumnDelegate(int type, QStyledItemDelegate* delegate);

    int columnForHeaderType(int headerType) const;
    void setColumnTypesVisible(const QList<int>& headerTypes, bool visible, bool exclusive = false);
    void setColumnsVisible(const QList<int>& columns, bool visible, bool exclusive = false);

public slots:
    virtual void refreshVisibleColumns(const QList<int>& columns);
    virtual void refreshVisibleIndexes(const QModelIndexList& indexes);
    virtual void refreshIndex(const QModelIndex& sourceIndex);
    virtual void clear();

protected:
    EntityMetadata findCurrentParent(int entityMetadataType) const;
    EntityMetadata findFirstParent(const QModelIndex& index, int entityMetadataType) const;
    QModelIndexList findParents(const QModelIndex& index) const;
    QModelIndexList mapToSource(const QModelIndexList& indexes) const;


    virtual void addHeaderContextMenuItems(QMenu* menu, const QPoint& globalPos) { Q_UNUSED(menu) Q_UNUSED(globalPos) }

    static void logIndex(const char* file, int lineNumber, Log::LogLevel level, const QModelIndex& index, const QString& text);
    static bool testMatch(const QModelIndex& index, int role, const QVariant &value, Qt::MatchFlags flags, QModelIndexList& foundIndexes);

private:
    AbstractItemModel* _sourceModel = nullptr;
    QSortFilterProxyModel* _proxyModel = nullptr;
    QMap<int, QStyledItemDelegate*> _columnDelegates;

    QAction* _actionColSettings = nullptr;
    QAction* _actionHideCol = nullptr;
    QAction* _actionAutoResizeCols = nullptr;
    QAction* _actionResetCols = nullptr;

    QPoint _contextMenuPos;

signals:
    void itemProgramaticallySelected(const QModelIndex& index);
    void headerChanged();
    void currentSelectionChanged();
    void currentIndexChanged(const QModelIndex& index);

    void entityAdded(const EntityMetadata& metadata);
    void entityDeleted(const EntityMetadata& metadata);
    void entityUpdated(const EntityMetadata& metadata);

private slots:
    virtual void onHorizontalHeaderResized(int /*logicalIndex*/, int /*oldSize*/, int /*newSize*/);

    void onHeaderContextMenuRequested();
    void onColumnSettingsClicked();
    void onHideColumnClicked();
    void onAutoResizeColumnsClicked();
    void onResetColumnsClicked();
    void onCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);
};

#endif // TREEVIEWBASE_H
