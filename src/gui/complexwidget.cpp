#include "complexwidget.h"

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
