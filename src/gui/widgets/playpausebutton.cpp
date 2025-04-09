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

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(_textLabel);
    layout->addWidget(_button);
    setLayout(layout);

    setPlaying(false);

    connect(_button, &QPushButton::clicked, this, &PlayPauseButton::onPlayPauseClicked);
}

void PlayPauseButton::setPlaying(bool value)
{
    _playing = value;
    _button->setIcon(_playing ? Resources::getIcon(Resources::Pause) : Resources::getIcon(Resources::Play));
    emit playingChanged(_playing);
}

void PlayPauseButton::setText(const QString& value)
{
    _text = value;
    _textLabel->setText(value);
}

void PlayPauseButton::onPlayPauseClicked()
{
    setPlaying(!_playing);
    emit clicked();
}

