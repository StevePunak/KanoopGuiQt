/******************************************************************************************
**
** abstracttablemodel.h
**
** Extends AbstractItemModel to provide some table-specific functions.
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Wed Oct 25 14:21:04 2023
**
******************************************************************************************/
#ifndef ABSTRACTTABLEMODEL_H
#define ABSTRACTTABLEMODEL_H
#include "abstractitemmodel.h"

class LIBKANOOPGUI_EXPORT AbstractTableModel : public AbstractItemModel
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
