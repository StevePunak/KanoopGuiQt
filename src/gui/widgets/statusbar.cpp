#include "widgets/statusbar.h"

#include <utility/stylesheet.h>
#include <Kanoop/log.h>


StatusBar::StatusBar(QWidget *parent) :
    QStatusBar(parent)
{
    connect(&_dotTimer, &QTimer::timeout, this, &StatusBar::onDotTimerExpired);
}

void StatusBar::showStatusMessage(const QString& text, const QColor& textColor, const TimeSpan& timeout)
{
    stopAnimation();
    setForegroundColor(textColor);
    QStatusBar::showMessage(text, timeout.totalMilliseconds());
}

void StatusBar::showAnimatedProgressMessage(const QString& text, const QColor& textColor)
{
Log::logText(LVL_DEBUG, QString("%1 [%2]").arg(__FUNCTION__).arg(text));
    _progressMessage = text;
    showStatusMessage(text, textColor);
    _dotTimer.start(250);
}

void StatusBar::stopAnimation()
{
Log::logText(LVL_DEBUG, __FUNCTION__);
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
Log::logText(LVL_DEBUG, "Dot timer...");
    QString text = QString("%1%2").arg(_progressMessage).arg(QString().leftJustified(++_dots, '.'));
    QStatusBar::showMessage(text);

    static const int DotCount = 25;
    if(_dots > DotCount) {
        _dots = 0;
    }
}
