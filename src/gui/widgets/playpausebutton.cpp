#include "widgets/playpausebutton.h"
#include "resources.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

PlayPauseButton::PlayPauseButton(QWidget *parent) :
    QWidget(parent)
{
    _textLabel = new QLabel("Label", this);

    _button = new QPushButton(this);
    _button->setMaximumSize(_textLabel->height(), _textLabel->height());

    _iconWhilePlaying = Resources::getIcon(Resources::Pause);
    _iconWhilePaused = Resources::getIcon(Resources::Play);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(_textLabel);
    layout->addWidget(_button);
    setLayout(layout);

    setPlaying(false);

    connect(_button, &QPushButton::clicked, this, &PlayPauseButton::onPlayPauseClicked);
}

bool PlayPauseButton::isTextVisible() const
{
    return _textLabel->isVisible();
}

void PlayPauseButton::setPlaying(bool value, bool blockSignal)
{
    _playing = value;
    _button->setIcon(_playing ? _iconWhilePlaying : _iconWhilePaused);
    if(blockSignal == false) {
        emit playingChanged(_playing);
    }
}

void PlayPauseButton::setText(const QString& value)
{
    _text = value;
    _textLabel->setText(value);
}

void PlayPauseButton::setTextVisible(bool value)
{
    _textLabel->setVisible(value);
}

void PlayPauseButton::setTextAlignment(Qt::Alignment value)
{
    _textAlignment = value;
    _textLabel->setAlignment(value);
}

void PlayPauseButton::onPlayPauseClicked()
{
    setPlaying(!_playing);
    emit clicked();
}

void PlayPauseButton::setButtonSize(const QSize& min, const QSize& max, const QSize& icon)
{
    _button->setMinimumSize(min);
    _button->setMaximumSize(max);
    _button->setIconSize(icon);
}
