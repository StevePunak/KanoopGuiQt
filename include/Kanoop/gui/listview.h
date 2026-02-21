#ifndef LISTVIEW_H
#define LISTVIEW_H
#include <QListView>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class QSortFilterProxyModel;
class AbstractItemModel;

/**
 * @brief QListView subclass that integrates with AbstractItemModel and adds UUID-based navigation.
 *
 * ListView automatically wraps the source model in a QSortFilterProxyModel and
 * provides helpers for locating and selecting items by UUID.
 */
class LIBKANOOPGUI_EXPORT ListView : public QListView,
                                     public LoggingBaseClass
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit ListView(QWidget* parent = nullptr);

    /**
     * @brief Set the model, wrapping it in an internal sort/filter proxy.
     * @param model Model to display (should derive from AbstractItemModel)
     */
    virtual void setModel(QAbstractItemModel* model) override;

    /**
     * @brief Return the first index whose item UUID matches.
     * @param uuid UUID to search for
     * @return First matching index, or invalid index if not found
     */
    virtual QModelIndex firstIndexOfEntityUuid(const QUuid& uuid) const;

    /**
     * @brief Return all indexes whose item UUID matches.
     * @param uuid UUID to search for
     * @return List of matching indexes
     */
    virtual QModelIndexList indexesOfUuid(const QUuid& uuid) const;

    /**
     * @brief Select the item with the given UUID and optionally scroll to it.
     * @param uuid UUID of the item to select
     * @param scrollHint How to scroll to make the item visible
     */
    virtual void setCurrentUuid(const QUuid& uuid, ScrollHint scrollHint = EnsureVisible);

    /**
     * @brief Return the underlying AbstractItemModel (without the proxy).
     * @return Pointer to the source model
     */
    AbstractItemModel* sourceModel() const { return _sourceModel; }

    /**
     * @brief Return the internal sort/filter proxy model.
     * @return Pointer to the proxy model
     */
    QSortFilterProxyModel* proxyModel() const { return _proxyModel; }

private:
    AbstractItemModel* _sourceModel = nullptr;
    QSortFilterProxyModel* _proxyModel = nullptr;

signals:
    /** @brief Emitted when the current selection changes. */
    void currentSelectionChanged();
    /**
     * @brief Emitted when the current index changes.
     * @param index The newly current index
     */
    void currentIndexChanged(const QModelIndex& index);

private slots:
    /** @brief Internal handler forwarding selection changes to currentSelectionChanged(). */
    void onCurrentSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // LISTVIEW_H
