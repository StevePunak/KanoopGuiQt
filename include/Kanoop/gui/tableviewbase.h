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

    void restoreHeaderStates();
    void restoreHorizontalHeaderState();
    void restoreVerticalHeaderState();

    void setColumnDelegate(int type, QStyledItemDelegate* delegate);

private:
    AbstractItemModel* _sourceModel;
    QSortFilterProxyModel* _proxyModel;
    QMap<int, QStyledItemDelegate*> _columnDelegates;

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
};

#endif // TABLEVIEWBASE_H
