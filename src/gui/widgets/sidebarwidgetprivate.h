#ifndef SIDEBARWIDGETPRIVATE_H
#define SIDEBARWIDGETPRIVATE_H

#include <QStandardItem>
#include <QStyledItemDelegate>

#include <Kanoop/entitymetadata.h>


class SidebarPaintDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SidebarPaintDelegate(QObject *parent = nullptr) :
        QStyledItemDelegate(parent),
        _horizontalSpacing(0), _verticalSpacing(0) {}

    QMargins contentsMargins() const { return _contentsMargins; }
    void setContentsMargins(const QMargins& value) { _contentsMargins = value; }

    int horizontalSpacing() const { return _horizontalSpacing; }
    void setHorizontalSpacing(int value) { _horizontalSpacing = value; }

    int verticalSpacing() const { return _verticalSpacing; }
    void setVerticalSpacing(int value) { _verticalSpacing = value; }

    QSize iconSize() const { return _iconSize; }
    void setIconSize(const QSize& value) { _iconSize = value; }

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    inline QFont textFont(const QStyleOptionViewItem &option) const;
    inline QRect textRectangle(const QStyleOptionViewItem &option) const;

    QMargins _contentsMargins;
    int _horizontalSpacing;
    int _verticalSpacing;
    QSize _iconSize;
};

class SidebarItem : public QStandardItem
{
public:
    SidebarItem(int entityType, const QString& text, const QIcon& icon) :
        QStandardItem(icon, text),
        _entityType(entityType) {}

    int entityType() const { return _entityType; }

private:
    int _entityType;
    QIcon _standardIcon;
    QIcon _darkModeIcon;
};

#endif // SIDEBARWIDGETPRIVATE_H
