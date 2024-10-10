#ifndef TABBAR_H
#define TABBAR_H

#include <QTabBar>

class TabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit TabBar(QWidget *parent = nullptr);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;

signals:
    void tabCustomContextMenuRequested(int index);
};

#endif // TABBAR_H
