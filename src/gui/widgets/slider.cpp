#include "widgets/slider.h"

#include <QKeyEvent>
#include <QWheelEvent>


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

void Slider::keyPressEvent(QKeyEvent* event)
{
    if(_readOnly) {
        event->ignore();
    }
    else {
        QSlider::keyPressEvent(event);
    }
}

void Slider::keyReleaseEvent(QKeyEvent* event)
{
    if(_readOnly) {
        event->ignore();
    }
    else {
        QSlider::keyReleaseEvent(event);
    }
}

void Slider::wheelEvent(QWheelEvent* event)
{
    if(_readOnly) {
        event->ignore();
    }
    else {
        QSlider::wheelEvent(event);
    }
}

#include "Kanoop/gui/widgets/moc_slider.cpp"
