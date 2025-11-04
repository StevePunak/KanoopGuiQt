#ifndef PLAYPAUSEBUTTON_H
#define PLAYPAUSEBUTTON_H

#include <QWidget>
#include <Kanoop/gui/libkanoopgui.h>

class QLabel;
class QPushButton;
class LIBKANOOPGUI_EXPORT PlayPauseButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool playing READ isPlaying WRITE setPlaying NOTIFY playingChanged)
    Q_PROPERTY(bool textVisible READ isTextVisible WRITE setTextVisible)                // clazy:exclude=qproperty-without-notify
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QIcon iconWhilePlaying READ iconWhilePlaying WRITE setIconWhilePlaying)  // clazy:exclude=qproperty-without-notify
    Q_PROPERTY(QIcon iconWhilePaused READ iconWhilePaused WRITE setIconWhilePaused)     // clazy:exclude=qproperty-without-notify
    Q_PROPERTY(Qt::Alignment textAlignment READ textAlignment WRITE setTextAlignment)   // clazy:exclude=qproperty-without-notify

public:
    explicit PlayPauseButton(QWidget *parent = nullptr);

    bool isPlaying() const { return _playing; }
    QString text() const { return _text; }
    bool isTextVisible() const;
    void setButtonSize(const QSize& min, const QSize& max, const QSize& icon);
    Qt::Alignment textAlignment() const { return _textAlignment; }
    QIcon iconWhilePlaying() const { return _iconWhilePlaying; }
    QIcon iconWhilePaused() const { return _iconWhilePaused; }

public slots:
    void setPlaying(bool value, bool blockSignal = false);
    void setText(const QString& value);
    void setTextVisible(bool value);
    void setTextAlignment(Qt::Alignment value);
    void setIconWhilePlaying(const QIcon& value) { _iconWhilePlaying = value; }
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
    void clicked();
    void playingChanged(bool playing);
    void textChanged(bool playing);

private slots:
    void onPlayPauseClicked();
};

#endif // PLAYPAUSEBUTTON_H
