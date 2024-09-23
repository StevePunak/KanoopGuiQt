/******************************************************************************************
**
** abstracttablemodel.h
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
** Created: Wed Oct 25 14:21:04 2023
**
******************************************************************************************/
#ifndef ABSTRACTTABLEMODEL_H
#define ABSTRACTTABLEMODEL_H
#include "abstractitemmodel.h"

class AbstractTableModel : public AbstractItemModel
{
    Q_OBJECT
public:
    AbstractTableModel(QObject* parent = nullptr);
    AbstractTableModel(const QString& loggingCategory, QObject* parent = nullptr);

    void deleteRowAtIndex(const QModelIndex& index);

protected:
    // AbstractItemModel interface
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual void columnChangedAtRowIndex(const QModelIndex& rowIndex, int columnHeader);
};

#endif // ABSTRACTTABLEMODEL_H
