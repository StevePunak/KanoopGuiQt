#include "designerplugininterfaces.h"

#include <resources.h>

#include <widgets/playpausebutton.h>

QIcon PlayPauseButtonDesignerPlugin::icon() const
{
    return Resources::getIcon(Resources::Play);
}

QWidget* PlayPauseButtonDesignerPlugin::createWidget(QWidget* parent)
{
    return new PlayPauseButton(parent);
}
