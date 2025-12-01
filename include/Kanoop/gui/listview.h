#ifndef LISTVIEW_H
#define LISTVIEW_H
#include <QListView>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class QSortFilterProxyModel;
class AbstractItemModel;
class LIBKANOOPGUI_EXPORT ListView : public QListView,
                                     public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit ListView(QWidget* parent = nullptr);

    virtual void setModel(QAbstractItemModel* model) override;

    virtual QModelIndex firstIndexOfEntityUuid(const QUuid& uuid) const;
    virtual QModelIndexList indexesOfUuid(const QUuid& uuid) const;
    virtual void setCurrentUuid(const QUuid& uuid, ScrollHint scrollHint = EnsureVisible);

    AbstractItemModel* sourceModel() const { return _sourceModel; }
    QSortFilterProxyModel* proxyModel() const { return _proxyModel; }

private:
    AbstractItemModel* _sourceModel = nullptr;
    QSortFilterProxyModel* _proxyModel = nullptr;

signals:
    void currentSelectionChanged();
    void currentIndexChanged(const QModelIndex& index);

private slots:
    void onCurrentSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // LISTVIEW_H
