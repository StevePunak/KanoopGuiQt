/******************************************************************************************
**
** abstractitemmodel.h
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
#ifndef ABSTRACTITEMMODEL_H
#define ABSTRACTITEMMODEL_H
#include <QAbstractItemModel>
#include <Kanoop/gui/abstractmodelitem.h>
#include <Kanoop/gui/tableheader.h>
#include <Kanoop/utility/loggingbaseclass.h>

class LIBKANOOPGUI_EXPORT AbstractItemModel : public QAbstractItemModel,
                                              public LoggingBaseClass
{
    Q_OBJECT
public:
    AbstractItemModel(QObject* parent = nullptr);
    AbstractItemModel(const QString& loggingCategory, QObject* parent = nullptr);
    virtual ~AbstractItemModel() {
        qDeleteAll(_rootItems);
    }

    // EntityMetadata Helpers
    QModelIndexList indexesOfEntityType(int type) const;
    QModelIndexList indexesOfEntity(int type, const QVariant &data, int role = Qt::DisplayRole) const;
    QModelIndexList indexesOfEntityUuid(const QUuid& uuid) const;
    QModelIndex firstIndexOfEntityType(int type) const;
    QModelIndex firstIndexOfEntity(int type, const QVariant &data, int role = Qt::DisplayRole) const;
    QModelIndex firstIndexOfEntityUuid(const QUuid& uuid) const;
    QModelIndex firstIndexOfChildEntityType(const QModelIndex& parent, int type) const;

    TableHeader::List columnHeaders() const;
    TableHeader columnHeader(int column) const { return _columnHeadersIntMap.value(column); }
    TableHeader rowHeader(int row) const { return _rowHeadersIntMap.value(row); }
    int columnForHeader(int type) const;

    QModelIndexList getPersistentIndexes() const;

    // QAbstractItemModel interface
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual bool removeRows(int row, int count, const QModelIndex& parentIndex) override;

protected:
    // Retrieve root items
    AbstractModelItem::List rootItems() const { return _rootItems; }

    // Append new items
    AbstractModelItem* appendRootItem(AbstractModelItem* item);

    // Headers
    void appendColumnHeader(int type, const QString& text);
    void appendColumnHeader(int type, const QColor& columnTextColor, const QString& text);
    void appendAdHocColumnHeader(int type, const QString& value) { appendColumnHeader(type, value); }
    void appendRowHeader(int type, const QString& value = QString());
    void appendAdHocRowHeader(int type, const QString& value) { appendRowHeader(type, value); }

    // Colors
    void setColumnTextColor(int type, const QColor& color);

    // Delete Items
    void deleteRootItem(AbstractModelItem* item);
    void deleteRootItems(const QUuid& uuid);
    void deleteRootItems(const EntityMetadata& metadata);

    // Update Items
    void updateItemAtIndex(const QModelIndex& itemIndex, const EntityMetadata &metadata);
    void updateItemsAtIndexes(const QModelIndexList& indexes, const EntityMetadata &metadata);

    TableHeader::IntMap columnHeadersIntMap() const { return _columnHeadersIntMap; }
    TableHeader::IntMap rowHeadersIntMap() const { return _rowHeadersIntMap; }

    static QString toString(const QModelIndex& value);

private:
    void commonInit();

    AbstractModelItem::List _rootItems;

    TableHeader::IntMap _columnHeadersIntMap;
    TableHeader::StringMap _columnHeadersStringMap;
    TableHeader::IntMap _rowHeadersIntMap;
    TableHeader::StringMap _rowHeadersStringMap;

signals:
    void itemAdded(const EntityMetadata& metadata);
    void itemDeleted(const EntityMetadata& metadata);
    void itemUpdated(const EntityMetadata& metadata);

public slots:
    virtual void addItem(const EntityMetadata& metadata) { Q_UNUSED(metadata) }
    virtual void deleteItem(const EntityMetadata& metadata) { Q_UNUSED(metadata) }
    virtual void updateItem(const EntityMetadata& metadata) { Q_UNUSED(metadata) }
};

#endif // ABSTRACTITEMMODEL_H
