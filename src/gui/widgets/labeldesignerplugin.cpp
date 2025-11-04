#include "labeldesignerplugin.h"
#include <resources.h>

#include <widgets/label.h>

QIcon LabelDesignerPlugin::icon() const
{
    return Resources::getIcon(Resources::Label);
}

QWidget* LabelDesignerPlugin::createWidget(QWidget* parent)
{
    return new Label("Label", parent);
}
