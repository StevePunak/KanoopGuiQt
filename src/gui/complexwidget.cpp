#include "complexwidget.h"

#include <QComboBox>
#include <QLineEdit>
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

void ComplexWidget::stringChanged(const QString&)
{
    validate();
}
