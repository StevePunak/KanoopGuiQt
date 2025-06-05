#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>
#include <QTimer>
#include <Kanoop/gui/libkanoopgui.h>
#include <Kanoop/timespan.h>

class LIBKANOOPGUI_EXPORT StatusBar : public QStatusBar
{
    Q_OBJECT
public:
    explicit StatusBar(QWidget *parent = nullptr);

    void showStatusMessage(const QString &text, const TimeSpan& timeout);
    void showStatusMessage(const QString &text, const QColor& textColor);
    void showStatusMessage(const QString &text, const QColor& textColor, const TimeSpan& timeout);
    void showAnimatedProgressMessage(const QString &text, const QColor& textColor = QColor());
    void stopAnimation();
    void setForegroundColor(const QColor& color);

private:
    void reset();

    // animation stuff
    QTimer _dotTimer;
    int _dots = 0;
    QString _progressMessage;

private slots:
    void onDotTimerExpired();
};

#endif // STATUSBAR_H
