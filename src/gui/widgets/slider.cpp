#include "widgets/slider.h"


Slider::Slider(QWidget *parent) :
    QSlider(parent)
{
}

Slider::Slider(Qt::Orientation orientation, QWidget* parent) :
    QSlider(orientation, parent)
{
}

void Slider::mousePressEvent(QMouseEvent* event)
{
    if(_readOnly == false) {
        QSlider::mousePressEvent(event);
    }
}
