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

/**
 * @brief AbstractItemModel specialization for tabular (row/column) models.
 *
 * Overrides columnCount() and data() with table-aware implementations and adds
 * a hook for notifying when a specific cell changes.
 */
class LIBKANOOPGUI_EXPORT AbstractTableModel : public AbstractItemModel
{
    Q_OBJECT
public:
    /** @brief Construct with an optional parent. */
    AbstractTableModel(QObject* parent = nullptr);

    /**
     * @brief Construct with a logging category and optional parent.
     * @param loggingCategory Category name used for log output
     * @param parent Optional QObject parent
     */
    AbstractTableModel(const QString& loggingCategory, QObject* parent = nullptr);

    /**
     * @brief Remove the row at the given index from the model.
     * @param index Model index of the row to delete
     */
    void deleteRowAtIndex(const QModelIndex& index);

protected:
    // AbstractItemModel interface
    /** @brief Return the number of columns (based on registered column headers). */
    virtual int columnCount(const QModelIndex &parent) const override;
    /** @brief Return the data for the given index and role. */
    virtual QVariant data(const QModelIndex &index, int role) const override;

    /**
     * @brief Called when a specific column cell at rowIndex has changed.
     * @param rowIndex Row index of the changed cell
     * @param columnHeader Column header type of the changed cell
     */
    virtual void columnChangedAtRowIndex(const QModelIndex& rowIndex, int columnHeader);
};

#endif // ABSTRACTTABLEMODEL_H
