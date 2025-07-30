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
#include <QSplitter>
#include <QWindow>

#include <Kanoop/geometry/size.h>
#include <Kanoop/geometry/point.h>
#include <Kanoop/geometry/rectangle.h>

#include <Kanoop/logconsumer.h>

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

Dialog::~Dialog()
{
    if(_logConsumer != nullptr) {
        closeLogConsumer();
    }
}

void Dialog::commonInit()
{
    Dialog::setObjectName(Dialog::metaObject()->className());

    _buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
    connectButtonBoxSignals();

    _statusBar = new QStatusBar(this);
    _statusBar->setVisible(false);
    _statusBar->setMaximumHeight(_statusBar->height());

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
    QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::NoButton;
    if(_applyEnabled) {
        buttons |= QDialogButtonBox::Apply;
    }
    if(_cancelEnabled) {
        buttons |= QDialogButtonBox::Cancel;
    }
    if(_okEnabled) {
        buttons |= QDialogButtonBox::Ok;
    }
    _buttonBox->setStandardButtons(buttons);
}

void Dialog::closeLogConsumer()
{
    if(_logConsumer != nullptr) {
        Log::removeConsumer(_logConsumer);
        delete _logConsumer;
        _logConsumer = nullptr;
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
    _applyEnabled = value;
    setButtonBoxButtons();
    connectButtonBoxSignals();
}

void Dialog::setCancelEnabled(bool value)
{
    _cancelEnabled = value;
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

void Dialog::setLogHookEnabled(bool enabled)
{
    if(enabled) {
        closeLogConsumer();
        _logConsumer = new LogConsumer;
        connect(_logConsumer, &LogConsumer::logEntry, this, &Dialog::onLoggedItem);
        Log::addConsumer(_logConsumer);
    }
    else {
        closeLogConsumer();
    }
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

        QWidget* parentWidget = Dialog::parentWidget();
        QPoint restorePos = pos;

        // Ensure the point is not off the screen
        QScreen* restorePosScreen = QGuiApplication::screenAt(restorePos);
        if(restorePosScreen == nullptr) {
            logText(LVL_DEBUG, QString("The restore point is off the screen - centering in parent"));
            QRect parentRect = parentWidget != nullptr
                                               ? parentWidget->rect()
                                               : QGuiApplication::primaryScreen()->availableGeometry();
            restorePos = parentRect.center();
            restorePos.rx() -= (rect().width() / 2);
            restorePos.ry() -= (rect().height() / 2);
        }

        // If the dialog restoring position to a different screen than
        // the parent, center it in the parent.
        if(_restoreToParentScreen == true && parentWidget != nullptr && restorePosScreen != nullptr) {
            QScreen* parentScreen = parentWidget->window()->windowHandle()->screen();
            if(parentScreen != restorePosScreen) {
                logText(LVL_DEBUG, QString("The restore point is on a different screen than parent - centering in parent"));
                restorePos = parentWidget->mapToGlobal(parentWidget->rect().center());
                restorePos.rx() -= (rect().width() / 2);
                restorePos.ry() -= (rect().height() / 2);
            }
        }

        if(restorePos.isNull() == false && size.isNull() == false) {
            if(_persistSize) {
                resize(size);
            }
            if(_persistPosition) {
                move(restorePos);
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

    if(GuiSettings::globalInstance() != nullptr) {
        for(QSplitter* splitter : findChildren<QSplitter*>()) {
            GuiSettings::globalInstance()->restoreLastSplitterState(splitter);
            connect(splitter, &QSplitter::splitterMoved, this, &Dialog::onSplitterMoved);
        }
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

void Dialog::onLoggedItem(const Log::LogEntry& entry)
{
    loggedItem(entry);
}

void Dialog::onSplitterMoved()
{
    if(GuiSettings::globalInstance() != nullptr) {
        GuiSettings::globalInstance()->saveLastSplitterState(static_cast<QSplitter*>(sender()));
    }
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
