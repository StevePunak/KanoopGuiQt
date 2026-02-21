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

/**
 * @brief Non-modal toast notification manager overlaid on a parent widget.
 *
 * ToastManager creates ToastWidget pop-ups positioned over the parent widget
 * and fades them out after a configurable delay.  Separate foreground/background
 * colors can be set for normal messages and error messages.  The toast size and
 * position within the parent are also configurable.
 */
class LIBKANOOPGUI_EXPORT ToastManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Construct attached to a parent widget.
     * @param parent Widget over which toasts are displayed
     */
    explicit ToastManager(QWidget *parent);

    /**
     * @brief Display a normal-priority message toast.
     * @param text Message text to display
     */
    void message(const QString& text) { displayToast(text, _messageBackgroundColor, _messageForegroundColor); }

    /**
     * @brief Display an error-priority message toast.
     * @param text Error message text to display
     */
    void errorMessage(const QString& text) { displayToast(text, _errorBackgroundColor, _errorForegroundColor); }

    /**
     * @brief Display a toast with explicit colors.
     * @param text Message text to display
     * @param backgroundColor Background color for the toast
     * @param foregroundColor Foreground (text) color for the toast
     */
    void displayToast(const QString& text, const QColor& backgroundColor, const QColor& foregroundColor);

    /**
     * @brief Return the time after which the toast begins fading.
     * @return Begin-fade delay
     */
    TimeSpan beginFadeTime() const { return _beginFadeTime; }

    /**
     * @brief Set the time after which the toast begins fading.
     * @param value New begin-fade delay
     */
    void setBeginFadeTime(const TimeSpan& value) { _beginFadeTime = value; }

    /**
     * @brief Return the duration of the fade-out animation.
     * @return Fade duration
     */
    TimeSpan fadeTime() const { return _fadeTime; }

    /**
     * @brief Set the duration of the fade-out animation.
     * @param value New fade duration
     */
    void setFadeTime(TimeSpan value) { _fadeTime = value; }

    /**
     * @brief Return the foreground color for normal messages.
     * @return Normal message foreground color
     */
    QColor messageForegroundColor() const { return _messageForegroundColor; }

    /**
     * @brief Set the foreground color for normal messages.
     * @param value New foreground color
     */
    void setMessageForegroundColor(const QColor& value) { _messageForegroundColor = value; }

    /**
     * @brief Return the background color for normal messages.
     * @return Normal message background color
     */
    QColor messageBackgroundColor() const { return _messageBackgroundColor; }

    /**
     * @brief Set the background color for normal messages.
     * @param value New background color
     */
    void setMessageBackgroundColor(const QColor& value) { _messageBackgroundColor = value; }

    /**
     * @brief Return the foreground color for error messages.
     * @return Error message foreground color
     */
    QColor errorForegroundColor() const { return _errorForegroundColor; }

    /**
     * @brief Set the foreground color for error messages.
     * @param value New foreground color
     */
    void setErrorForegroundColor(const QColor& value) { _errorForegroundColor = value; }

    /**
     * @brief Return the background color for error messages.
     * @return Error message background color
     */
    QColor errorBackgroundColor() const { return _errorBackgroundColor; }

    /**
     * @brief Set the background color for error messages.
     * @param value New background color
     */
    void setErrorBackgroundColor(const QColor& value) { _errorBackgroundColor = value; }

    /**
     * @brief Return the fixed size of toast widgets.
     * @return Toast size
     */
    QSize size() const { return _size; }

    /**
     * @brief Set the fixed size of toast widgets.
     * @param value New toast size
     */
    void resize(const QSize& value) { _size = value; }

    /**
     * @brief Return the position of toasts within the parent widget.
     * @return Toast position in parent coordinates
     */
    QPoint pos() const { return _pos; }

    /**
     * @brief Set the position of toasts within the parent widget.
     * @param value New toast position in parent coordinates
     */
    void move(const QPoint& value) { _pos = value; }

private:
    void performLayout();
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    QWidget* _parentWidget;
    TimeSpan _beginFadeTime;
    TimeSpan _fadeTime;
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
