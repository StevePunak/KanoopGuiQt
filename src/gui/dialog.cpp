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

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QSpinBox>

#include <Kanoop/geometry/size.h>
#include <Kanoop/geometry/point.h>

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

    _statusBar = new QStatusBar(this);
    _statusBar->setVisible(false);

    Dialog::onPreferencesChanged();
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

void Dialog::connectRadioButtonSignals()
{
    for(QRadioButton* widget : findChildren<QRadioButton*>()) {
        connect(widget, &QRadioButton::toggled, this, &Dialog::boolChanged);
    }
}

void Dialog::connectCheckBoxSignals()
{
    for(QCheckBox* widget : findChildren<QCheckBox*>()) {
        connect(widget, &QCheckBox::toggled, this, &Dialog::boolChanged);
    }
}

void Dialog::connectSpinBoxSignals()
{
    for(QSpinBox* widget : findChildren<QSpinBox*>()) {
        connect(widget, &QSpinBox::valueChanged, this, &Dialog::intChanged);
    }
}

void Dialog::connectButtonBoxSignals()
{
    if(_buttonBox->button(QDialogButtonBox::Ok) != nullptr) {
        disconnect(_buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &Dialog::onOkClicked);
        connect(_buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &Dialog::onOkClicked);
    }

    if(_buttonBox->button(QDialogButtonBox::Apply) != nullptr) {
        disconnect(_buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &Dialog::onApplyClicked);
        connect(_buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &Dialog::onApplyClicked);
    }
    if(_buttonBox->button(QDialogButtonBox::Cancel) != nullptr) {
        disconnect(_buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &Dialog::onCancelClicked);
        connect(_buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &Dialog::onCancelClicked);
    }
}

void Dialog::setButtonBoxButtons()
{
    QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::Cancel;
    if(_applyEnabled) {
        buttons |= QDialogButtonBox::Apply;
    }
    if(_okEnabled) {
        buttons |= QDialogButtonBox::Ok;
    }
    _buttonBox->setStandardButtons(buttons);
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
    _applyEnabled = value;
    setButtonBoxButtons();
    connectButtonBoxSignals();
}

void Dialog::setOkEnabled(bool value)
{
    _okEnabled = value;
    setButtonBoxButtons();
    connectButtonBoxSignals();
}

void Dialog::setStatusBarVisible(bool value)
{
    _statusBar->setVisible(value);
}

void Dialog::connectValidationSignals()
{
    connectLineEditSignals();
    connectComboBoxSignals();
    connectRadioButtonSignals();
    connectCheckBoxSignals();
    connectSpinBoxSignals();
}

void Dialog::moveEvent(QMoveEvent *event)
{
    // logText(LVL_DEBUG, QString("%1 - move to %2").arg(objectName()).arg(Point(event->pos()).toString()));
    if(_formLoadComplete && _persistPosition) {
        GuiSettings::globalInstance()->setLastWindowPosition(this, event->pos());
    }
    QDialog::moveEvent(event);
}

void Dialog::resizeEvent(QResizeEvent *event)
{
    // logText(LVL_DEBUG, QString("%1 - resize to %2").arg(objectName()).arg(Size(event->size()).toString()));
    if(_formLoadComplete && isVisible() && _persistSize) {
        GuiSettings::globalInstance()->setLastWindowSize(this, event->size());
    }
    QDialog::resizeEvent(event);
}

void Dialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    if(!_formLoadComplete) {
        QPoint pos = GuiSettings::globalInstance()->getLastWindowPosition(this, _defaultSize);
        QSize size = GuiSettings::globalInstance()->getLastWindowSize(this, _defaultSize);
        if(pos.isNull() == false && size.isNull() == false) {
            if(_persistSize) {
                resize(size);
            }
            if(_persistPosition) {
                move(pos);
            }
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
        myLayout->addWidget(_statusBar);
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
