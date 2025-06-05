#include "widgets/statusbar.h"

#include <utility/stylesheet.h>
#include <Kanoop/log.h>


StatusBar::StatusBar(QWidget *parent) :
    QStatusBar(parent)
{
    connect(&_dotTimer, &QTimer::timeout, this, &StatusBar::onDotTimerExpired);
}

void StatusBar::showStatusMessage(const QString& text, const TimeSpan& timeout)
{
    stopAnimation();
    QStatusBar::showMessage(text, timeout.totalMilliseconds());
}

void StatusBar::showStatusMessage(const QString& text, const QColor& textColor)
{
    showStatusMessage(text, textColor, TimeSpan());
}

void StatusBar::showStatusMessage(const QString& text, const QColor& textColor, const TimeSpan& timeout)
{
    stopAnimation();
    setForegroundColor(textColor);
    QStatusBar::showMessage(text, timeout.totalMilliseconds());
}

void StatusBar::showAnimatedProgressMessage(const QString& text, const QColor& textColor)
{
    _progressMessage = text;
    showStatusMessage(text, textColor);
    _dotTimer.start(250);
}

void StatusBar::stopAnimation()
{
    _dotTimer.stop();
}

void StatusBar::setForegroundColor(const QColor& color)
{
    QColor c = color.isValid() ? color : palette().color(QPalette::Text);
    StyleSheet<QStatusBar> ss;
    ss.setProperty(SP_Color, c);
    setStyleSheet(ss.toString());
}

void StatusBar::reset()
{
    stopAnimation();
    setForegroundColor(QColor());
}

void StatusBar::onDotTimerExpired()
{
    QString text = QString("%1%2").arg(_progressMessage).arg(QString().leftJustified(++_dots, '.'));
    QStatusBar::showMessage(text);

    static const int DotCount = 25;
    if(_dots > DotCount) {
        _dots = 0;
    }
}
