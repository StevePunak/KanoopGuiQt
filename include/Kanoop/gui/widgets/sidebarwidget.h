#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QListView>
#include <Kanoop/entitymetadata.h>
#include <Kanoop/gui/libkanoopgui.h>

class QStandardItemModel;
class SidebarPaintDelegate;
class LIBKANOOPGUI_EXPORT SidebarWidget : public QListView
{
    Q_OBJECT
public:
    explicit SidebarWidget(QWidget *parent = nullptr);

    void addItem(int entityMetadataType, const QString& text, int imageResourceId = 0);
    void removeItem(int entityMetadataType);
    bool containsItem(int entityMetadataType) const;
    void clear();
    void selectItem(int entityMetadataType);

    QMargins contentsMargins() const;
    void setContentsMargins(const QMargins& value);

    int horizontalSpacing() const;
    void setHorizontalSpacing(int value);

    int verticalSpacing() const;
    void setVerticalSpacing(int value);

    QSize iconSize() const;
    void setIconSize(const QSize& value);

private:
    QStandardItemModel* sourceModel() const;

    SidebarPaintDelegate* _delegate;

    static const QSize DefaultIconSize;
    static const QMargins DefaultContentsMargins;
    static const int DefaultVerticalSpacing;
    static const int DefaultHorizontalSpacing;

signals:
    void itemClicked(int entityType);

private slots:
    void onItemClicked(const QModelIndex& index);
};

#endif // SIDEBARWIDGET_H
