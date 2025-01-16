#include "accordionwidgetprivate.h"

#include <QPainter>
#include <QVBoxLayout>

#include <Kanoop/gui/resources.h>

AccordionItem::AccordionItem(const QString& title, QWidget* parent) :
    QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    _button = new DropDownButton(title);
    _button->setCheckable(true);
    layout->addWidget(_button);

    _centralWidget = new QWidget;
    _centralWidget->hide();
    layout->addWidget(_centralWidget);

    connect(_button, &QPushButton::toggled, this, &AccordionItem::showHideCentralWidget);
}

void AccordionItem::setExpanded(bool expanded)
{
    if(expanded) {
        _centralWidget->show();
    }
    else {
        _centralWidget->hide();
    }
    _button->setExpanded(expanded);
}

void AccordionItem::setTitle(const QString& title)
{
    _button->setText(title);
}

void AccordionItem::showHideCentralWidget(bool checked)
{
    setExpanded(checked);
}

DropDownButton::DropDownButton(const QString& text, QWidget* parent) :
    QPushButton(text, parent)
{
    _up = Resources::getPixmap(Resources::DropDownUp);
    _down = Resources::getPixmap(Resources::DropDownDown);
}

void DropDownButton::setExpanded(bool value)
{
    _expanded = value;
    update();
}

void DropDownButton::paintEvent(QPaintEvent* event)
{
    static const int margin = 6;

    QPushButton::paintEvent(event);
    int iconSize = height() - (margin * 2);
    QRect paintRect(margin, margin, iconSize, iconSize);
    QPainter painter(this);
    QPixmap pm = _expanded ? _down : _up;
    painter.drawPixmap(paintRect, pm);
}
