/******************************************************************************************
**
** treeviewbase.h
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
** Created: Sun Oct 22 17:41:53 2023
**
******************************************************************************************/
#ifndef TREEVIEWBASE_H
#define TREEVIEWBASE_H
#include "tableheader.h"

#include <Kanoop/entitymetadata.h>

#include <QTreeView>

class QStyledItemDelegate;
class AbstractItemModel;
class TreeViewBase : public QTreeView
{
    Q_OBJECT
public:
    TreeViewBase(QWidget* parent = nullptr);

    int entityTypeAtPos(const QPoint& pos);
    EntityMetadata currentMetadata() const;
    EntityMetadata metadataAtPos(const QPoint& pos) const;
    void setCurrentUuid(const QUuid& uuid);

    QByteArray saveState() const;
    void restoreState(const QByteArray& state);

    void restoreHeaderStates();

    AbstractItemModel* sourceModel() const;

    void setColumnDelegate(int type, QStyledItemDelegate* delegate);

protected:
    EntityMetadata findCurrentParent(int entityMetadataType) const;
    QModelIndexList findParents(const QModelIndex& index) const;

private:
    QMap<int, QStyledItemDelegate*> _columnDelegates;

signals:
    void itemProgramaticallySelected(const QModelIndex& index);
    void headerChanged();

private slots:
    virtual void onHorizontalHeaderResized(int /*logicalIndex*/, int /*oldSize*/, int /*newSize*/);
};

#endif // TREEVIEWBASE_H
