#ifndef ABSTRACTLISTMODEL_H
#define ABSTRACTLISTMODEL_H
#include "abstractitemmodel.h"

class LIBKANOOPGUI_EXPORT AbstractListModel : public AbstractItemModel
{
    Q_OBJECT
public:
    AbstractListModel(QObject* parent = nullptr);
    AbstractListModel(const QString& loggingCategory, QObject* parent = nullptr);

    void deleteRowAtIndex(const QModelIndex& index);

protected:
    // AbstractItemModel interface
    virtual QVariant data(const QModelIndex &index, int role) const override;
};

#endif // ABSTRACTLISTMODEL_H
