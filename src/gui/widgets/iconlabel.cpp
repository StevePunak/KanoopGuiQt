#include "widgets/iconlabel.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>



IconLabel::IconLabel(QWidget* parent) :
    QWidget(parent)
{
    createLayout();
}

IconLabel::IconLabel(const QString& text, QWidget* parent) :
    QWidget(parent)
{
    createLayout();
    setText(text);
}

IconLabel::IconLabel(const QString& text, const QIcon& icon, QWidget* parent) :
    QWidget(parent)
{
    createLayout();
    setText(text);
    setIcon(icon);
}

QString IconLabel::text() const
{
    return _label->text();
}

void IconLabel::setText(const QString& text)
{
    _label->setText(text);
}

QIcon IconLabel::icon() const
{
    return QIcon(_iconLabel->pixmap());
}

void IconLabel::setIcon(const QIcon& icon)
{
    _iconLabel->setPixmap(icon.pixmap(_iconLabel->size()));
}

void IconLabel::setIconAlignment(Qt::Alignment alignment)
{
    layout()->setAlignment(_iconLabel, alignment);
}

void IconLabel::createLayout()
{
    QFontMetrics fm(font());
    int dimension = fm.boundingRect("o").height();

    _label = new QLabel(this);
    _iconLabel = new QLabel(this);
    _iconLabel->setFixedSize(dimension, dimension);
    _iconLabel->setScaledContents(true);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_iconLabel);
    layout->addWidget(_label);
}

#include "Kanoop/gui/widgets/moc_iconlabel.cpp"
