/******************************************************************************************
**
** tableviewbase.h
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#ifndef TABLEVIEWBASE_H
#define TABLEVIEWBASE_H
#include <Kanoop/entitymetadata.h>
#include <Kanoop/utility/loggingbaseclass.h>

#include <QTableView>
#include <Kanoop/gui/libkanoopgui.h>

class QStyledItemDelegate;
class QSortFilterProxyModel;
class AbstractItemModel;
class LIBKANOOPGUI_EXPORT TableViewBase : public QTableView,
                                          public LoggingBaseClass
{
    Q_OBJECT

public:
    explicit TableViewBase(QWidget* parent = nullptr);
    virtual ~TableViewBase();

    virtual void setModel(QAbstractItemModel* model) override;

    int entityTypeAtPos(const QPoint& pos);
    EntityMetadata currentMetadata() const;
    EntityMetadata metadataAtPos(const QPoint& pos) const;

    AbstractItemModel* sourceModel() const { return _sourceModel; }
    QSortFilterProxyModel* proxyModel() const { return _proxyModel; }

    virtual void deleteRow(const QModelIndex& index);
    virtual void addRow(const EntityMetadata& metadata) { emit entityAdded(metadata); }
    int rowCount() const;

    QModelIndexList allRows() const;

    QModelIndex findFirstMatch(const QVariant& needle, int role) const;

    void setCurrentUuid(const QUuid& uuid, ScrollHint scrollHint = EnsureVisible);

    bool isIndexVisible(const QModelIndex& index) const;

    void restoreHeaderStates();
    void restoreHorizontalHeaderState();
    void restoreVerticalHeaderState();

    void setColumnDelegate(int type, QStyledItemDelegate* delegate);

public slots:
    void clear();

private:
    AbstractItemModel* _sourceModel;
    QSortFilterProxyModel* _proxyModel;
    QMap<int, QStyledItemDelegate*> _columnDelegates;

    QAction* _actionColSettings = nullptr;
    QAction* _actionHideCol = nullptr;
    QAction* _actionAutoResizeCols = nullptr;
    QAction* _actionResetCols = nullptr;

    QPoint _contextMenuPoint;

signals:
    void horizontalHeaderChanged();
    void verticalHeaderChanged();

    void currentSelectionChanged();
    void currentIndexChanged(const QModelIndex& current, const QModelIndex& previous);

    void entityAdded(const EntityMetadata& metadata);
    void entityDeleted(const EntityMetadata& metadata);
    void entityUpdated(const EntityMetadata& metadata);

protected slots:
    virtual void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;

private slots:
    virtual void onHorizontalHeaderResized(int /*logicalIndex*/, int /*oldSize*/, int /*newSize*/);
    virtual void onVerticalHeaderResized(int /*logicalIndex*/, int /*oldSize*/, int /*newSize*/);

    void onHeaderContextMenuRequested(const QPoint& pos);
    void onColumnSettingsClicked();
    void onHideColumnClicked();
    void onAutoResizeColumnsClicked();
    void onResetColumnsClicked();
};

#endif // TABLEVIEWBASE_H
