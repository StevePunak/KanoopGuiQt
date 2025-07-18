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
    virtual void addRow(const EntityMetadata& metadata) { emit addItem(metadata); }
    int rowCount() const;

    QModelIndex findFirstMatch(const QVariant& needle, int role) const;

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

signals:
    void horizontalHeaderChanged();
    void verticalHeaderChanged();

    void currentSelectionChanged();

    // Model updates
    void itemAdded(const EntityMetadata& metadata);
    void itemDeleted(const EntityMetadata& metadata);
    void itemUpdated(const EntityMetadata& metadata);

    void addItem(const EntityMetadata& metadata);
    void deleteItem(const EntityMetadata& metadata);
    void updateItem(const EntityMetadata& metadata);

private slots:
    virtual void onHorizontalHeaderResized(int /*logicalIndex*/, int /*oldSize*/, int /*newSize*/);
    virtual void onVerticalHeaderResized(int /*logicalIndex*/, int /*oldSize*/, int /*newSize*/);

    void onHeaderContextMenuRequested();
    void onColumnSettingsClicked();
    void onHideColumnClicked();
    void onAutoResizeColumnsClicked();
    void onResetColumnsClicked();
};

#endif // TABLEVIEWBASE_H
