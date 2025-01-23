#ifndef ACCORDIONWIDGET_H
#define ACCORDIONWIDGET_H

#include <QPushButton>
#include <QWidget>
#include <Kanoop/gui/libkanoopgui.h>

class AccordionItem;
class LIBKANOOPGUI_EXPORT AccordionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AccordionWidget(QWidget *parent = nullptr);

    void addItem(const QString& title, QWidget* content);
    void insertItem(int index, const QString& title, QWidget* content);
    void clear();

    void expandAll();
    void collapseAll();
    void setExpanded(QWidget* widget, bool expanded);

    void setTitle(QWidget* widget, const QString& title);

    int indexOf(QWidget* widget) const;

    int count() const;

private:
    AccordionItem* findItemForWidget(QWidget* widget);

signals:

};

#endif // ACCORDIONWIDGET_H
