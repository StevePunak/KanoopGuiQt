#ifndef WIDGETPLUGINCOLLECTION_H
#define WIDGETPLUGINCOLLECTION_H

#include <QtDesigner>
#include <QtCore>

class WidgetPluginCollection : public QObject,
                               public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)public:

public:
    explicit WidgetPluginCollection(QObject *parent = nullptr);

    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const override;

private:
    QList<QDesignerCustomWidgetInterface*> _widgets;
};

#endif // WIDGETPLUGINCOLLECTION_H
