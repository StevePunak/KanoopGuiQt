#include "widgets/spinnerwidget.h"

#include <QPaintEvent>
#include <QPainter>
#include <resources.h>

#include <Kanoop/geometry/point.h>
#include <Kanoop/geometry/rectangle.h>
#include <Kanoop/geometry/size.h>

const double SpinnerWidget::DegreesPerTick = 10;

namespace Colors = QColorConstants::Svg;

SpinnerWidget::SpinnerWidget(QWidget* parent) :
    QFrame(parent),
    _value(0), _index(0), _textVisible(false)
{
    connect(&_timer, &QTimer::timeout, this, &SpinnerWidget::onSpinTimer);
}

void SpinnerWidget::setSpinning(bool value)
{
    if(value) {
        _timer.start(100);
    }
    else {
        _timer.stop();
    }
}

void SpinnerWidget::buildPixmaps()
{
    Size pixmapSize(qMin(size().width(), size().height()), qMin(size().width(), size().height()));
    _pixmaps.clear();

    double LineWidth = pixmapSize.width() / 8;
    double SpanAngle = 60;

    int count = 360 / DegreesPerTick;
    for(int i = 0;i < count;i++) {
        double degrees = i * DegreesPerTick;

        QPixmap pixmap(pixmapSize.toSize());
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setPen(QPen(Colors::grey, LineWidth));

        Rectangle arcRect = pixmap.rect();
        arcRect.shrink(LineWidth / 2);
        painter.drawArc(arcRect, degrees * 16, SpanAngle * 16);

        painter.setPen(QPen(Colors::lightgrey, LineWidth));
        painter.drawArc(arcRect, (degrees + SpanAngle) * 16, (360 - SpanAngle) * 16);

        _pixmaps.append(pixmap);
    }

    QFont f = font();
    f.setPixelSize((pixmapSize.height() / 2));
    setFont(f);
}

void SpinnerWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    if(_pixmaps.count() == 0) {
        return;
    }

    QString text = QString("%1").arg(_value);
    QPainter painter(this);
    painter.drawPixmap(rect(), _pixmaps.at(_index));

    if(_textVisible) {
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, text);
    }
}

void SpinnerWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    buildPixmaps();
}

void SpinnerWidget::onSpinTimer()
{
    _index = _index < _pixmaps.count() - 1 ? _index + 1 : 0;
    update();
}
