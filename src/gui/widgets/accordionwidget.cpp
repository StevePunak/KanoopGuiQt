#include "widgets/accordionwidget.h"

#include "accordionwidgetprivate.h"

#include <QPainter>
#include <QVBoxLayout>
#include <guisettings.h>

AccordionWidget::AccordionWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
}

void AccordionWidget::addItem(const QString& title, QWidget* content)
{
    QVBoxLayout* layout = static_cast<QVBoxLayout*>(AccordionWidget::layout());
    int count = layout->count();
    insertItem(count - 1, title, content);
    emit itemCountModified();
}

void AccordionWidget::insertItem(int index, const QString& title, QWidget* content)
{
    AccordionItem* item = new AccordionItem(title, this);
    item->centralWidget()->setLayout(new QVBoxLayout);
    item->centralWidget()->layout()->addWidget(content);

    QVBoxLayout* layout = static_cast<QVBoxLayout*>(AccordionWidget::layout());
    layout->insertWidget(index, item);
    emit itemCountModified();
}

void AccordionWidget::removeItem(int index)
{
    QVBoxLayout* layout = static_cast<QVBoxLayout*>(AccordionWidget::layout());
    QLayoutItem* item = layout->takeAt(index);
    delete item->widget();
    emit itemCountModified();
}

void AccordionWidget::setItemVisible(int index, bool visible)
{
    QList<AccordionItem*> items = findChildren<AccordionItem*>(Qt::FindDirectChildrenOnly);
    if(items.count() > index) {
        items[index]->setVisible(visible);
    }
}

void AccordionWidget::clear()
{
    QList<AccordionItem*> items = findChildren<AccordionItem*>(Qt::FindDirectChildrenOnly);
    qDeleteAll(items);
    emit itemCountModified();
}

void AccordionWidget::expandAll()
{
    QList<AccordionItem*> items = findChildren<AccordionItem*>(Qt::FindDirectChildrenOnly);
    for(AccordionItem* item : items) {
        item->setExpanded(true);
    }
}

void AccordionWidget::collapseAll()
{
    QList<AccordionItem*> items = findChildren<AccordionItem*>(Qt::FindDirectChildrenOnly);
    for(AccordionItem* item : items) {
        item->setExpanded(false);
    }
}

void AccordionWidget::setExpanded(QWidget* widget, bool expanded)
{
    AccordionItem* item = findItemForWidget(widget);
    if(item != nullptr) {
        item->setExpanded(expanded);
    }
}

void AccordionWidget::setTitle(QWidget* widget, const QString& title)
{
    AccordionItem* item = findItemForWidget(widget);
    if(item != nullptr) {
        item->setTitle(title);
    }
}

int AccordionWidget::indexOf(QWidget* widget) const
{
    int result = -1;
    QVBoxLayout* layout = static_cast<QVBoxLayout*>(AccordionWidget::layout());
    for(int i = 0;i < layout->count();i++) {
        AccordionItem* item = qobject_cast<AccordionItem*>(layout->itemAt(i)->widget());
        if(item != nullptr && item->centralWidget()->children().contains(widget)) {
            result = i;
            break;
        }
    }
    return result;
}

int AccordionWidget::count() const
{
    QVBoxLayout* layout = static_cast<QVBoxLayout*>(AccordionWidget::layout());
    int count = layout->count() - 1;
    return count;
}

AccordionItem* AccordionWidget::findItemForWidget(QWidget* widget)
{
    AccordionItem* result = nullptr;
    for(AccordionItem* item : findChildren<AccordionItem*>()) {
        if(item->centralWidget()->children().contains(widget)) {
            result = item;
            break;
        }
    }
    return result;
}

void AccordionWidget::onPreferencesChanged()
{
    if(GuiSettings::globalInstance() == nullptr) {
        return;
    }

    QFont newFont(font());
    int pointSize = GuiSettings::globalInstance()->fontSize();
    newFont.setPointSize(pointSize);
    setFont(newFont);
}

