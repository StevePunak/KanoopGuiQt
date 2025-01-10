#include "widgets/widgetplugincollection.h"
#include "widgets/playpausebutton.h"
#include "designerplugininterfaces.h"

WidgetPluginCollection::WidgetPluginCollection(QObject *parent) :
    QObject(parent)
{
    _widgets.append(new PlayPauseButtonDesignerPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> WidgetPluginCollection::customWidgets() const
{
    return _widgets;
}
