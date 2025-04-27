/******************************************************************************************
**
** abstractitemmodel.h
**
** Extends the native Qt QAbstractItemModel to provide additional funtionality which
** I've found useful over the years.
**
** Most of the methods provide a default implementation of QAbstractItemModel and most
** other methods as self-explainitory if you are familiar with the Qt Abstract Model.
**
** It is intended to work with objects inherited from AbstractModelItem.
**
** Moved from my Tesseract Engineering repo to open-source
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
    QModelIndex firstMatch(const QModelIndex& startSearchIndex, int role, const QVariant& value, Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const;

    TableHeader::List columnHeaders() const;
    TableHeader columnHeader(int column) const { return _columnHeaders.value(column); }
    TableHeader rowHeader(int row) const { return _rowHeaders.value(row); }
    int columnForHeader(int type) const;

    QModelIndexList getPersistentIndexes() const;

    void refresh(const QModelIndex& topLeft, const QModelIndex& bottomRight);

    // QAbstractItemModel interface
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual bool removeRows(int row, int count, const QModelIndex& parentIndex) override;
    virtual bool hasChildren(const QModelIndex& parent) const override;

    void setColumnHeaderVisible(int type, bool visible);

protected:
    // Retrieve root items
    AbstractModelItem::List rootItems() const { return _rootItems; }
    const AbstractModelItem::List rootItemsConst() const { return _rootItems; }
    int rootItemCount() const { return _rootItems.count(); }

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

    TableHeader::IntMap columnHeadersIntMap() const { return _columnHeaders; }
    TableHeader::IntMap rowHeadersIntMap() const { return _rowHeaders; }

    void emitRowChanged(const QModelIndex &rowIndex);

    static QString toString(const QModelIndex& index);

private:
    void commonInit();

    AbstractModelItem::List _rootItems;

    // class Headers : public QMap<

    TableHeader::IntMap _columnHeaders;
    TableHeader::IntMap _rowHeaders;

    friend class AbstractModelItem;

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
