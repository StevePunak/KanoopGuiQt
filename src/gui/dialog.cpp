/******************************************************************************************
**
** dialogbase.cpp
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#include "dialog.h"
#include "guisettings.h"
#include <QLayout>
#include <QMoveEvent>
#include <QPushButton>
#include <QResizeEvent>
#include <QShowEvent>
#include <QTimer>

#include <QComboBox>
#include <QLineEdit>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    LoggingBaseClass("dlg"),
    _formLoadComplete(false), _formLoadFailed(false),
    _valid(false), _dirty(false)
{
    commonInit();
}

Dialog::Dialog(const QString &loggingCategory, QWidget *parent) :
    QDialog(parent),
    LoggingBaseClass(loggingCategory),
    _formLoadComplete(false), _formLoadFailed(false),
    _valid(false), _dirty(false)
{
    commonInit();
}

void Dialog::commonInit()
{
    Dialog::setObjectName(Dialog::metaObject()->className());

    _buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
    connectButtonBoxSignals();

    Dialog::onPreferencesChanged();
}

void Dialog::moveEvent(QMoveEvent *event)
{
    if(_formLoadComplete) {
        GuiSettings::globalInstance()->setLastWindowPosition(this, event->pos());
    }
    QDialog::moveEvent(event);
}

void Dialog::resizeEvent(QResizeEvent *event)
{
    if(_formLoadComplete) {
        GuiSettings::globalInstance()->setLastWindowSize(this, event->size());
    }
    QDialog::resizeEvent(event);
}

void Dialog::connectLineEditSignals()
{
    for(QLineEdit* widget : findChildren<QLineEdit*>()) {
        connect(widget, &QLineEdit::textChanged, this, &Dialog::stringChanged);
    }
}

void Dialog::connectComboBoxSignals()
{
    for(QComboBox* widget : findChildren<QComboBox*>()) {
        connect(widget, &QComboBox::currentTextChanged, this, &Dialog::stringChanged);
    }
}

void Dialog::connectButtonBoxSignals()
{
    if(_buttonBox->button(QDialogButtonBox::Ok) != nullptr) {
        connect(_buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &Dialog::onOkClicked);
    }

    if(_buttonBox->button(QDialogButtonBox::Apply) != nullptr) {
        connect(_buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &Dialog::onApplyClicked);
    }
    if(_buttonBox->button(QDialogButtonBox::Cancel) != nullptr) {
        connect(_buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &Dialog::onCancelClicked);
    }
}

void Dialog::setValid(bool value)
{
    _valid = value;
}

void Dialog::setDirty(bool value)
{
    _dirty = value;
}

void Dialog::setApplyEnabled(bool value)
{
    if(value == false) {
        _buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    }
    else {
        _buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
    }
    connectButtonBoxSignals();
}

void Dialog::connectValidationSignals()
{
    connectLineEditSignals();
    connectComboBoxSignals();
}

void Dialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    if(!_formLoadComplete) {
        QPoint pos = GuiSettings::globalInstance()->getLastWindowPosition(this);
        QSize size = GuiSettings::globalInstance()->getLastWindowSize(this);
        if(pos.isNull() == false && size.isNull() == false) {
            setGeometry(QRect(pos, size));
        }
        _formLoadComplete = true;
        if(_formLoadFailed) {
            QTimer::singleShot(0, this, &QWidget::close);
        }
    }
    QDialog::showEvent(event);
}

void Dialog::performLayout()
{
    if(layout() != nullptr) {
        // should we check to ensure vertical layout?
        QLayout* myLayout = layout();
        myLayout->addWidget(_buttonBox);
    }
}

void Dialog::enableAppropriateButtons()
{
    validate();
    if(_buttonBox->button(QDialogButtonBox::Ok) != nullptr) {
        _buttonBox->button(QDialogButtonBox::Ok)->setEnabled(_valid);
    }

    if(_buttonBox->button(QDialogButtonBox::Apply) != nullptr) {
        _buttonBox->button(QDialogButtonBox::Apply)->setEnabled(_valid && _dirty);
    }
    if(_buttonBox->button(QDialogButtonBox::Cancel) != nullptr) {
        _buttonBox->button(QDialogButtonBox::Cancel)->setEnabled(true);
    }
}

void Dialog::onPreferencesChanged()
{
    logText(LVL_DEBUG, QString("%1").arg(__FUNCTION__));
    if(GuiSettings::globalInstance() == nullptr) {
        return;
    }

    QFont newFont(font());
    newFont.setPointSize(GuiSettings::globalInstance()->fontSize());
    setFont(newFont);
}

void Dialog::stringChanged(const QString &)
{
    enableAppropriateButtons();
}

void Dialog::intChanged(int)
{
    enableAppropriateButtons();
}

void Dialog::boolChanged(bool)
{
    enableAppropriateButtons();
}

void Dialog::voidChanged()
{
    enableAppropriateButtons();
}

void Dialog::onOkClicked()
{
    okClicked();
    accept();
}

void Dialog::onApplyClicked()
{
    applyClicked();
    enableAppropriateButtons();
}

void Dialog::onCancelClicked()
{
    cancelClicked();
    reject();
}
