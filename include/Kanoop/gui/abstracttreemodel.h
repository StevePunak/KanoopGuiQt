/******************************************************************************************
**
** abstracttreemodel.h
**
** Extends AbstractItemModel to provide some tree-specific functions.
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Tue Oct 10 09:37:57 2023
**
******************************************************************************************/
#ifndef ABSTRACTTREEMODEL_H
#define ABSTRACTTREEMODEL_H
#include "abstractitemmodel.h"

class LIBKANOOPGUI_EXPORT AbstractTreeModel : public AbstractItemModel
{
    Q_OBJECT
public:
    AbstractTreeModel(QObject* parent = nullptr);
    AbstractTreeModel(const QString& loggingCategory, QObject* parent = nullptr);

protected:
    // AbstractItemModel interface
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual void columnChangedAtRowIndex(const QModelIndex& rowIndex, int columnHeader);
};

#endif // ABSTRACTTREEMODEL_H
