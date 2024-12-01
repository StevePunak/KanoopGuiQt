#include "widgets/iconlabel.h"

#include <QHBoxLayout>
#include <QLabel>



IconLabel::IconLabel(QWidget* parent) :
    QWidget(parent)
{
    createLayout(QString(), QIcon());
}

IconLabel::IconLabel(const QString& text, QWidget* parent) :
    QWidget(parent)
{
    createLayout(text, QIcon());
}

IconLabel::IconLabel(const QString& text, const QIcon& icon, QWidget* parent) :
    QWidget(parent)
{
    createLayout(text, icon);
}

QString IconLabel::text() const
{
    return _label->text();
}

void IconLabel::setText(const QString& text)
{
    createLayout(text, icon());
}

QIcon IconLabel::icon() const
{
    return QIcon(_iconLabel->pixmap());
}

void IconLabel::setIcon(const QIcon& icon)
{
    createLayout(text(), icon);
}

void IconLabel::createLayout(const QString& text, const QIcon& icon)
{
    if(_label != nullptr) {
        delete _label;
    }
    if(_iconLabel != nullptr) {
        delete _iconLabel;
    }

    QFontMetrics fm(font());
    int dimension = fm.boundingRect("o").height();

    _label = new QLabel(text, this);
    _iconLabel = new QLabel(this);
    _iconLabel->setFixedSize(dimension, dimension);
    _iconLabel->setScaledContents(true);
    _iconLabel->setPixmap(icon.pixmap(_iconLabel->size()));

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_iconLabel);
    layout->addWidget(_label);

    setLayout(layout);
}
