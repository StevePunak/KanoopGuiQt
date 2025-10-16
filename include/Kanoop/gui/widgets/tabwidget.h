#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);

signals:
    void tabCustomContextMenuRequested(int index);
};

#endif // TABWIDGET_H
