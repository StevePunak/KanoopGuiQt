#ifndef ABSTRACTLISTMODEL_H
#define ABSTRACTLISTMODEL_H
#include "abstractitemmodel.h"

/**
 * @brief AbstractItemModel specialization for flat list models.
 *
 * Provides a data() override suitable for list views and adds a convenience
 * method for deleting the item at a given index.
 */
class LIBKANOOPGUI_EXPORT AbstractListModel : public AbstractItemModel
{
    Q_OBJECT
public:
    /** @brief Construct with an optional parent. */
    AbstractListModel(QObject* parent = nullptr);

    /**
     * @brief Construct with a logging category and optional parent.
     * @param loggingCategory Category name used for log output
     * @param parent Optional QObject parent
     */
    AbstractListModel(const QString& loggingCategory, QObject* parent = nullptr);

    /**
     * @brief Remove the row at the given index from the model.
     * @param index Model index of the row to delete
     */
    void deleteRowAtIndex(const QModelIndex& index);

protected:
    // AbstractItemModel interface
    /** @brief Return the data for the given index and role. */
    virtual QVariant data(const QModelIndex &index, int role) const override;
};

#endif // ABSTRACTLISTMODEL_H
