/******************************************************************************************
**
** tableviewbase.h
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
#ifndef TABLEVIEWBASE_H
#define TABLEVIEWBASE_H
#include <Kanoop/entitymetadata.h>
#include <Kanoop/utility/loggingbaseclass.h>

#include <QTableView>

class QStyledItemDelegate;
class QSortFilterProxyModel;
class AbstractItemModel;
class TableViewBase : public QTableView,
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
