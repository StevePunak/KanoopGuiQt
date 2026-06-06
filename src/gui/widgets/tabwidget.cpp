#include "widgets/tabwidget.h"

#include "tabbar.h"

TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    TabBar* tabBar = new TabBar(this);
    connect(tabBar, &TabBar::tabCustomContextMenuRequested, this, &TabWidget::tabCustomContextMenuRequested);
    setTabBar(tabBar);
}

#include "Kanoop/gui/widgets/moc_tabwidget.cpp"
