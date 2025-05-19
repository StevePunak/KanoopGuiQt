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

    AbstractItemModel* sourceModel() const { return _sourceModel; }
    QSortFilterProxyModel* proxyModel() const { return _proxyModel; }

private:
    AbstractItemModel* _sourceModel = nullptr;
    QSortFilterProxyModel* _proxyModel = nullptr;

signals:
    void currentSelectionChanged();
};

#endif // LISTVIEW_H
