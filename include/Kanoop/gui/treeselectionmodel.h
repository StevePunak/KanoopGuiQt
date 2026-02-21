#ifndef TREESELECTIONMODEL_H
#define TREESELECTIONMODEL_H
#include <Kanoop/gui/libkanoopgui.h>

#include <QItemSelectionModel>

#include <Kanoop/utility/loggingbaseclass.h>

/**
 * @brief QItemSelectionModel subclass with logging support for tree views.
 *
 * Overrides select() to allow subclasses or debug instrumentation to intercept
 * selection changes made on a tree view.
 */
class LIBKANOOPGUI_EXPORT TreeSelectionModel : public QItemSelectionModel,
                                               public LoggingBaseClass
{
    Q_OBJECT
public:
    /**
     * @brief Construct with a model and optional parent.
     * @param model The item model this selection model operates on
     * @param parent Optional QObject parent
     */
    TreeSelectionModel(QAbstractItemModel *model, QObject *parent = nullptr);

public slots:
    /**
     * @brief Select a single index with the given selection flags.
     * @param index Index to select
     * @param command Selection flags controlling how the selection is modified
     */
    virtual void select(const QModelIndex& index, QItemSelectionModel::SelectionFlags command) override;

    /**
     * @brief Apply a selection change described by a QItemSelection.
     * @param selection The selection to apply
     * @param command Selection flags controlling how the selection is modified
     */
    virtual void select(const QItemSelection& selection, QItemSelectionModel::SelectionFlags command) override;
};

#endif // TREESELECTIONMODEL_H
