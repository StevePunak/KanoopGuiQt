#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>
#include <QTimer>
#include <Kanoop/gui/libkanoopgui.h>
#include <Kanoop/timespan.h>

/**
 * @brief QStatusBar subclass with timed messages, color styling, and animated progress text.
 *
 * StatusBar extends QStatusBar with:
 * - showStatusMessage() overloads that accept a TimeSpan timeout and/or a text color
 * - showAnimatedProgressMessage() which appends animated dots until stopAnimation() is called
 * - setForegroundColor() for permanent foreground color overrides
 */
class LIBKANOOPGUI_EXPORT StatusBar : public QStatusBar
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit StatusBar(QWidget *parent = nullptr);

    /**
     * @brief Show a message that disappears after a timeout.
     * @param text Message text
     * @param timeout Duration before the message is cleared
     */
    void showStatusMessage(const QString &text, const TimeSpan& timeout);

    /**
     * @brief Show a colored message with no automatic timeout.
     * @param text Message text
     * @param textColor Text color
     */
    void showStatusMessage(const QString &text, const QColor& textColor);

    /**
     * @brief Show a colored message that disappears after a timeout.
     * @param text Message text
     * @param textColor Text color
     * @param timeout Duration before the message is cleared
     */
    void showStatusMessage(const QString &text, const QColor& textColor, const TimeSpan& timeout);

    /**
     * @brief Show a message with animated trailing dots to indicate progress.
     * @param text Base message text (dots are appended automatically)
     * @param textColor Optional text color (invalid QColor = default)
     */
    void showAnimatedProgressMessage(const QString &text, const QColor& textColor = QColor());

    /** @brief Stop the animated progress message and clear the status bar. */
    void stopAnimation();

    /**
     * @brief Set a permanent foreground color for status bar text.
     * @param color Foreground color
     */
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
