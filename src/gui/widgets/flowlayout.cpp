#include "widgets/flowlayout.h"
#include <Kanoop/log.h>
#include <QWidget>

FlowLayout::FlowLayout(QWidget *parent) :
    QGridLayout{parent},
    _maxColumns(15)
{
}

void FlowLayout::addWidget(QWidget *widget)
{
    int currentCount = count();
    int row = currentCount / _maxColumns;
    int col = currentCount - (row * _maxColumns);
    setColumnMinimumWidth(col, widget->width() + horizontalSpacing());
    QGridLayout::addWidget(widget, row, col);

    int width = (widget->width() * _maxColumns) + (horizontalSpacing() * _maxColumns);
    int height = (widget->height() * (row + 1)) + (verticalSpacing() * (row + 1));

    setGeometry(QRect(0, 0, width, height));
}

void FlowLayout::compact()
{
    int row = rowCount() - 1;
    int col = columnCount() - 1;
    if(row >= 0 && col >= 0) {
        addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed), row, col);
    }
}

void FlowLayout::clear()
{
    while(count() > 0) {
        QLayoutItem* item = takeAt(0);
        QWidget* widget = item->widget();
        // Item before widget: ~QWidgetItemV2 dereferences its widget, and QWidget's
        // destructor clears that back-pointer only on the item Qt registered for it.
        delete item;
        delete widget;
    }
}

#include "Kanoop/gui/widgets/moc_flowlayout.cpp"
