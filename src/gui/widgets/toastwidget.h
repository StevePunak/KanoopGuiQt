#ifndef TOASTWIDGET_H
#define TOASTWIDGET_H

#include <QFrame>

#include <Kanoop/timespan.h>

class QGraphicsOpacityEffect;
class QToolButton;
class Label;
class ToastWidget : public QFrame
{
    Q_OBJECT
public:
    ToastWidget(const QString& text, const QColor& backgroundColor, const QColor& foregroundColor, const TimeSpan& beginFadeTime, const TimeSpan& fadeTime, QWidget* parent = nullptr);

    QDateTime createTime() const { return _createTime; }

private:
    virtual void resizeEvent(QResizeEvent* event) override;

    Label* _label;
    QToolButton* _closeButton;
    QDateTime _createTime;
    QGraphicsOpacityEffect* _opacityEffect = nullptr;

    QColor _backgroundColor;
    QColor _foregroundColor;

    double _opacity = 1.0;

    TimeSpan _fadeSleepTime;
    double _opacityQuanta = 0;

public:
    static const QColor DefaultMessageForeground;
    static const QColor DefaultMessageBackground;
    static const QColor DefaultErrorForeground;
    static const QColor DefaultErrorBackground;

signals:
    void complete();

private slots:
    void onCloseTimer();
    void onClick();
};

#endif // TOASTWIDGET_H
