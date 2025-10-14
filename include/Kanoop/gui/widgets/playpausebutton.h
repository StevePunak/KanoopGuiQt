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
    Q_PROPERTY(bool textVisible READ isTextVisible WRITE setTextVisible)                  // clazy:exclude=qproperty-without-notify
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    explicit PlayPauseButton(QWidget *parent = nullptr);

    bool isPlaying() const { return _playing; }
    QString text() const { return _text; }
    bool isTextVisible() const;
    void setButtonSize(QSize min, QSize max, QSize icon);

public slots:
    void setPlaying(bool value);
    void setText(const QString& value);
    void setTextVisible(bool value);

private:
    QPushButton* _button;
    QLabel* _textLabel;

    bool _playing = false;
    QString _text;

signals:
    void clicked();
    void playingChanged(bool playing);
    void textChanged(bool playing);

private slots:
    void onPlayPauseClicked();
};

#endif // PLAYPAUSEBUTTON_H
