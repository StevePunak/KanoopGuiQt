#include "tabbar.h"

#include <QMouseEvent>

TabBar::TabBar(QWidget *parent) :
    QTabBar(parent)
{
}

void TabBar::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::RightButton && parentWidget()->contextMenuPolicy() == Qt::CustomContextMenu) {
        int index = tabAt(event->pos());
        if(index >= 0) {
            emit tabCustomContextMenuRequested(index);
        }
    }
    QTabBar::mousePressEvent(event);
}
