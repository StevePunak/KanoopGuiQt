#include "designerplugininterfaces.h"

#include <resources.h>

#include <widgets/playpausebutton.h>
#include <widgets/pushbutton.h>

QIcon PlayPauseButtonDesignerPlugin::icon() const
{
    return Resources::getIcon(Resources::Play);
}

QWidget* PlayPauseButtonDesignerPlugin::createWidget(QWidget* parent)
{
    return new PlayPauseButton(parent);
}

QIcon PushButtonDesignerPlugin::icon() const
{
    return QIcon();
}

QWidget* PushButtonDesignerPlugin::createWidget(QWidget* parent)
{
    return new PushButton(parent);
}
