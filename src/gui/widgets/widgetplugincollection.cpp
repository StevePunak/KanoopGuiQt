#include "widgets/widgetplugincollection.h"
#include "widgets/playpausebutton.h"
#include "checkboxdesignerplugin.h"
#include "designerplugininterfaces.h"
#include "labeldesignerplugin.h"

WidgetPluginCollection::WidgetPluginCollection(QObject *parent) :
    QObject(parent)
{
    _widgets.append(new PlayPauseButtonDesignerPlugin(this));
    _widgets.append(new LabelDesignerPlugin(this));
    _widgets.append(new CheckBoxDesignerPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> WidgetPluginCollection::customWidgets() const
{
    return _widgets;
}
