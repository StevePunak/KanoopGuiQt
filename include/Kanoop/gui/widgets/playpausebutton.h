#ifndef PLAYPAUSEBUTTON_H
#define PLAYPAUSEBUTTON_H

#include <QWidget>
#include <Kanoop/gui/libkanoopgui.h>

class QLabel;
class QPushButton;

/**
 * @brief Compound widget that toggles between "play" and "pause" states.
 *
 * PlayPauseButton displays a QPushButton whose icon switches between
 * iconWhilePlaying and iconWhilePaused according to the current playing state.
 * An optional text label can be shown alongside the button with configurable
 * alignment.  The playing property is fully bindable and notifies via
 * playingChanged().
 */
class LIBKANOOPGUI_EXPORT PlayPauseButton : public QWidget
{
    Q_OBJECT
    /** @brief Whether the button is in the playing state. */
    Q_PROPERTY(bool playing READ isPlaying WRITE setPlaying NOTIFY playingChanged)
    /** @brief Whether the companion text label is visible. */
    Q_PROPERTY(bool textVisible READ isTextVisible WRITE setTextVisible)                // clazy:exclude=qproperty-without-notify
    /** @brief Text shown in the companion label. */
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    /** @brief Icon displayed while the button is in the playing state. */
    Q_PROPERTY(QIcon iconWhilePlaying READ iconWhilePlaying WRITE setIconWhilePlaying)  // clazy:exclude=qproperty-without-notify
    /** @brief Icon displayed while the button is in the paused state. */
    Q_PROPERTY(QIcon iconWhilePaused READ iconWhilePaused WRITE setIconWhilePaused)     // clazy:exclude=qproperty-without-notify
    /** @brief Alignment of the companion text label. */
    Q_PROPERTY(Qt::Alignment textAlignment READ textAlignment WRITE setTextAlignment)   // clazy:exclude=qproperty-without-notify

public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit PlayPauseButton(QWidget *parent = nullptr);

    /**
     * @brief Return whether the widget is in the "playing" state.
     * @return true if playing
     */
    bool isPlaying() const { return _playing; }

    /**
     * @brief Return the companion text label's text.
     * @return Label text string
     */
    QString text() const { return _text; }

    /**
     * @brief Return whether the companion text label is visible.
     * @return true if visible
     */
    bool isTextVisible() const;

    /**
     * @brief Set minimum, maximum, and icon sizes on the button.
     * @param min Minimum button size
     * @param max Maximum button size
     * @param icon Icon size
     */
    void setButtonSize(const QSize& min, const QSize& max, const QSize& icon);

    /**
     * @brief Return the alignment of the companion text label.
     * @return Qt alignment flags
     */
    Qt::Alignment textAlignment() const { return _textAlignment; }

    /**
     * @brief Return the icon shown while playing.
     * @return Playing icon
     */
    QIcon iconWhilePlaying() const { return _iconWhilePlaying; }

    /**
     * @brief Return the icon shown while paused.
     * @return Paused icon
     */
    QIcon iconWhilePaused() const { return _iconWhilePaused; }

public slots:
    /**
     * @brief Set the playing state and optionally suppress the signal.
     * @param value true for playing, false for paused
     * @param blockSignal true to suppress playingChanged()
     */
    void setPlaying(bool value, bool blockSignal = false);

    /**
     * @brief Set the companion text label text.
     * @param value New text string
     */
    void setText(const QString& value);

    /**
     * @brief Show or hide the companion text label.
     * @param value true to show, false to hide
     */
    void setTextVisible(bool value);

    /**
     * @brief Set the text label alignment.
     * @param value Qt alignment flags
     */
    void setTextAlignment(Qt::Alignment value);

    /**
     * @brief Set the icon displayed while in the playing state.
     * @param value New playing icon
     */
    void setIconWhilePlaying(const QIcon& value) { _iconWhilePlaying = value; }

    /**
     * @brief Set the icon displayed while in the paused state.
     * @param value New paused icon
     */
    void setIconWhilePaused(const QIcon& value) { _iconWhilePaused = value; }

private:
    QPushButton* _button;
    QLabel* _textLabel;

    QIcon _iconWhilePlaying;
    QIcon _iconWhilePaused;

    bool _playing = false;
    QString _text;
    Qt::Alignment _textAlignment = Qt::AlignLeft;

signals:
    /** @brief Emitted when the button is clicked. */
    void clicked();
    /**
     * @brief Emitted when the playing state changes.
     * @param playing New playing state
     */
    void playingChanged(bool playing);
    /**
     * @brief Emitted when the text label text changes.
     * @param playing New playing state at time of text change
     */
    void textChanged(bool playing);

private slots:
    void onPlayPauseClicked();
};

#endif // PLAYPAUSEBUTTON_H
