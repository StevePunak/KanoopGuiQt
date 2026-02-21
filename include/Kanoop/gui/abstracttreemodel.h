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

/**
 * @brief AbstractItemModel specialization for hierarchical (tree) models.
 *
 * Adds a columnChangedAtRowIndex() hook for notifying subclasses when a
 * cell's column data changes during a tree-model update.
 */
class LIBKANOOPGUI_EXPORT AbstractTreeModel : public AbstractItemModel
{
    Q_OBJECT
public:
    /** @brief Construct with an optional parent. */
    AbstractTreeModel(QObject* parent = nullptr);

    /**
     * @brief Construct with a logging category and optional parent.
     * @param loggingCategory Category name used for log output
     * @param parent Optional QObject parent
     */
    AbstractTreeModel(const QString& loggingCategory, QObject* parent = nullptr);

protected:
    // AbstractItemModel interface
    /**
     * @brief Called when a specific column cell at rowIndex has changed.
     * @param rowIndex Row index of the changed cell
     * @param columnHeader Column header type of the changed cell
     */
    virtual void columnChangedAtRowIndex(const QModelIndex& rowIndex, int columnHeader);
};

#endif // ABSTRACTTREEMODEL_H
