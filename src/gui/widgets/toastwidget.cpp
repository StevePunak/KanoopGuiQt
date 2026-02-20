#include "toastwidget.h"

#include <QGraphicsOpacityEffect>
#include <QResizeEvent>
#include <QTimer>
#include <QToolButton>
#include <resources.h>
#include <stylesheets.h>

#include <utility/stylesheet.h>
#include <Kanoop/log.h>

#include <widgets/label.h>

namespace Colors = QColorConstants::Svg;

const QColor ToastWidget::DefaultMessageForeground      = Colors::black;
const QColor ToastWidget::DefaultMessageBackground      = Colors::lightgreen;
const QColor ToastWidget::DefaultErrorForeground        = Colors::white;
const QColor ToastWidget::DefaultErrorBackground        = Colors::red;

ToastWidget::ToastWidget(const QString& text, const QColor& backgroundColor, const QColor& foregroundColor, const TimeSpan& beginFadeTime, const TimeSpan& fadeTime, QWidget* parent) :
    QFrame(parent),
    _createTime(QDateTime::currentDateTimeUtc()),
    _backgroundColor(backgroundColor),
    _foregroundColor(foregroundColor),
    _opacity(1.0)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, false);

    _closeButton = new QToolButton(this);
    _closeButton->setIcon(Resources::getIcon(Resources::CloseButton));
    _closeButton->setFixedSize(16, 16);
    _closeButton->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    connect(_closeButton, &QToolButton::clicked, this, &ToastWidget::complete);
    connect(_closeButton, &QToolButton::clicked, this, &ToastWidget::onClick);

    _label = new Label(text, this);
    _label->setWordWrap(true);
    _label->setAlignment(Qt::AlignCenter);
    _label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

    _opacityEffect = new QGraphicsOpacityEffect(this);
    _opacityEffect->setOpacity(_opacity);
    setGraphicsEffect(_opacityEffect);

    StyleSheet<QFrame> ss;
    ss.setProperty(SP_BackgroundColor, backgroundColor);
    ss.setProperty(SP_Color, foregroundColor);
    ss.setPropertyPixels(SP_BorderRadius, 10);
    setStyleSheet(ss.toString());

    _label->setStyleSheet(styleSheet());
    _closeButton->setStyleSheet(styleSheet());

    // calculate fade sleep
    _fadeSleepTime = TimeSpan::fromMilliseconds(50);
    double steps = fadeTime.totalMilliseconds() / _fadeSleepTime.totalMilliseconds();
    _opacityQuanta = 1.0 / steps;

    QTimer::singleShot(beginFadeTime.totalMilliseconds(), this, &ToastWidget::onCloseTimer);
}

void ToastWidget::resizeEvent(QResizeEvent* event)
{
    static const int CloseButtonOffset = 2;
    static const int LabelMargin = 4;
    QFrame::resizeEvent(event);
    QSize size = event->size().shrunkBy(QMargins(LabelMargin, LabelMargin, LabelMargin, LabelMargin));
    _label->resize(size);
    _label->move(LabelMargin, LabelMargin);
    QPoint pos((event->size().width() - _closeButton->width()) - CloseButtonOffset, CloseButtonOffset);
    _closeButton->move(pos);
    _closeButton->raise();
}

void ToastWidget::onCloseTimer()
{
    _opacity -= _opacityQuanta;
    if(_opacity <= 0) {
        emit complete();
    }
    else {
        _opacityEffect->setOpacity(_opacity);
        update();
        QTimer::singleShot(_fadeSleepTime.totalMilliseconds(), this, &ToastWidget::onCloseTimer);
    }
}

void ToastWidget::onClick()
{
    Log::logText(LVL_DEBUG, "Clicked");
}
