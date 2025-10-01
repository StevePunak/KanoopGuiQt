#ifndef TREESELECTIONMODEL_H
#define TREESELECTIONMODEL_H
#include <Kanoop/gui/libkanoopgui.h>

#include <QItemSelectionModel>

#include <Kanoop/utility/loggingbaseclass.h>

class LIBKANOOPGUI_EXPORT TreeSelectionModel : public QItemSelectionModel,
                                               public LoggingBaseClass
{
    Q_OBJECT
public:
    TreeSelectionModel(QAbstractItemModel *model, QObject *parent = nullptr);

public slots:
    virtual void select(const QModelIndex& index, QItemSelectionModel::SelectionFlags command) override;
    virtual void select(const QItemSelection& selection, QItemSelectionModel::SelectionFlags command) override;
};

#endif // TREESELECTIONMODEL_H
