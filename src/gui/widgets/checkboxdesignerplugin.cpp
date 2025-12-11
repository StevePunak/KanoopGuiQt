#include "checkboxdesignerplugin.h"
#include <resources.h>

#include <widgets/checkbox.h>

QIcon CheckBoxDesignerPlugin::icon() const
{
    return Resources::getIcon(Resources::CheckBox);
}

QWidget* CheckBoxDesignerPlugin::createWidget(QWidget* parent)
{
    return new CheckBox("Some checkbox", parent);
}
