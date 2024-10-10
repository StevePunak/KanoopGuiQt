#ifndef TOASTMANAGER_H
#define TOASTMANAGER_H

#include <QDateTime>
#include <QFrame>

#include <Kanoop/timespan.h>
#include <Kanoop/gui/libkanoopgui.h>

class QGraphicsOpacityEffect;
class Label;
class QToolButton;
class QLabel;
class ToastWidget;
class LIBKANOOPGUI_EXPORT ToastManager : public QObject
{
    Q_OBJECT
public:
    explicit ToastManager(QWidget *parent);
    void message(const QString& text) { displayToast(text, _messageBackgroundColor, _messageForegroundColor); }
    void errorMessage(const QString& text) { displayToast(text, _errorBackgroundColor, _errorForegroundColor); }
    void displayToast(const QString& text, const QColor& backgroundColor, const QColor& foregroundColor);

    TimeSpan autoCloseTime() const { return _autoCloseTime; }
    void setAutoCloseTime(const TimeSpan& value) { _autoCloseTime = value; }

    QColor messageForegroundColor() const { return _messageForegroundColor; }
    void setMessageForegroundColor(const QColor& value) { _messageForegroundColor = value; }

    QColor messageBackgroundColor() const { return _messageBackgroundColor; }
    void setMessageBackgroundColor(const QColor& value) { _messageBackgroundColor = value; }

    QColor errorForegroundColor() const { return _errorForegroundColor; }
    void setErrorForegroundColor(const QColor& value) { _errorForegroundColor = value; }

    QColor errorBackgroundColor() const { return _errorBackgroundColor; }
    void setErrorBackgroundColor(const QColor& value) { _errorBackgroundColor = value; }

    QSize size() const { return _size; }
    void resize(const QSize& value) { _size = value; }

    QPoint pos() const { return _pos; }
    void move(const QPoint& value) { _pos = value; }

private:
    void performLayout();

    QWidget* _parentWidget;
    TimeSpan _autoCloseTime;
    QPoint _pos;
    QSize _size;

    QColor _messageBackgroundColor;
    QColor _messageForegroundColor;
    QColor _errorBackgroundColor;
    QColor _errorForegroundColor;

    QList<ToastWidget*> _toasts;

signals:

private slots:
    void closeToast();
};

#endif // TOASTMANAGER_H
