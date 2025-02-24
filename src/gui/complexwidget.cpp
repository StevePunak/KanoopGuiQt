#include "complexwidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QSpinBox>
#include <QSplitter>
#include <guisettings.h>

ComplexWidget::ComplexWidget(QWidget *parent) :
    QWidget(parent),
    LoggingBaseClass(QString())
{
    commonInit();
}

ComplexWidget::ComplexWidget(const QString& loggingCategory, QWidget* parent) :
    QWidget(parent),
    LoggingBaseClass(loggingCategory)
{
    commonInit();
}

void ComplexWidget::commonInit()
{
}

void ComplexWidget::initializeBase()
{
    onPreferencesChanged();

    if(GuiSettings::globalInstance() != nullptr) {
        for(QSplitter* splitter : findChildren<QSplitter*>()) {
            GuiSettings::globalInstance()->restoreLastSplitterState(splitter);
            connect(splitter, &QSplitter::splitterMoved, this, &ComplexWidget::onSplitterMoved);
        }
    }
}

void ComplexWidget::connectValidationSignals()
{
    connectLineEditSignals();
    connectComboBoxSignals();
    connectRadioButtonSignals();
    connectCheckBoxSignals();
    connectSpinBoxSignals();
}

void ComplexWidget::connectLineEditSignals()
{
    for(QLineEdit* widget : findChildren<QLineEdit*>()) {
        connect(widget, &QLineEdit::textChanged, this, &ComplexWidget::stringChanged);
    }
}

void ComplexWidget::connectComboBoxSignals()
{
    for(QComboBox* widget : findChildren<QComboBox*>()) {
        connect(widget, &QComboBox::currentTextChanged, this, &ComplexWidget::stringChanged);
    }
}

void ComplexWidget::connectRadioButtonSignals()
{
    for(QRadioButton* widget : findChildren<QRadioButton*>()) {
        connect(widget, &QRadioButton::toggled, this, &ComplexWidget::boolChanged);
    }
}

void ComplexWidget::connectCheckBoxSignals()
{
    for(QCheckBox* widget : findChildren<QCheckBox*>()) {
        connect(widget, &QCheckBox::toggled, this, &ComplexWidget::boolChanged);
    }
}

void ComplexWidget::connectSpinBoxSignals()
{
    for(QSpinBox* widget : findChildren<QSpinBox*>()) {
        connect(widget, &QSpinBox::valueChanged, this, &ComplexWidget::intChanged);
    }
}

void ComplexWidget::onPreferencesChanged()
{
    if(GuiSettings::globalInstance() == nullptr) {
        return;
    }

    QFont newFont(font());
    int pointSize = GuiSettings::globalInstance()->fontSize();
    newFont.setPointSize(pointSize);
    setFont(newFont);
}

void ComplexWidget::onSplitterMoved()
{
    GuiSettings::globalInstance()->saveLastSplitterState(static_cast<QSplitter*>(sender()));
}


