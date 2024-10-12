#include "widgets/toastmanager.h"

#include "toastwidget.h"

#include <Kanoop/gui/utility/stylesheet.h>

#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QResizeEvent>
#include <QTimer>
#include <QToolButton>

#include <Kanoop/geometry/point.h>
#include <Kanoop/gui/resources.h>
#include <Kanoop/gui/widgets/label.h>
#include <Kanoop/log.h>

namespace Colors = QColorConstants::Svg;

ToastManager::ToastManager(QWidget *parent) :
    _parentWidget(parent),
    _beginFadeTime(TimeSpan::fromSeconds(3.5)),
    _fadeTime(TimeSpan::fromSeconds(10)),
    _messageBackgroundColor(ToastWidget::DefaultMessageBackground),
    _messageForegroundColor(ToastWidget::DefaultMessageForeground),
    _errorBackgroundColor(ToastWidget::DefaultErrorBackground),
    _errorForegroundColor(ToastWidget::DefaultErrorForeground)
{
}

void ToastManager::displayToast(const QString& text, const QColor& backgroundColor, const QColor& foregroundColor)
{
    ToastWidget* toast = new ToastWidget(text, backgroundColor, foregroundColor, _beginFadeTime, _fadeTime, _parentWidget);
    connect(toast, &ToastWidget::complete, this, &ToastManager::closeToast);
    _toasts.append(toast);

    QFontMetrics fm(toast->font());
    QRect boundingRect = fm.boundingRect(0, 0, _size.width(), 0, Qt::TextWordWrap, text);
    boundingRect.setHeight(boundingRect.height() + 48);     // close button
    boundingRect.setWidth(_size.width());
    toast->setFixedSize(boundingRect.size());
    toast->show();

    performLayout();
}

void ToastManager::performLayout()
{
    // sort oldest to newest
    std::sort(_toasts.begin(), _toasts.end(), [](const ToastWidget* a, const ToastWidget* b) { return a->createTime() > b->createTime(); });

    int y = _parentWidget->height() - 1;
    for(ToastWidget* toast : _toasts) {
        y -= toast->height() + 1;
        Point pos(0, y);
        toast->move(pos.toPoint());
    }
}

void ToastManager::closeToast()
{
    ToastWidget* toast = dynamic_cast<ToastWidget*>(sender());
    if(toast != nullptr) {
        _toasts.removeAll(toast);
        delete toast;
        performLayout();
    }
}

