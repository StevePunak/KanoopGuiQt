#ifndef WIDGETPLUGINCOLLECTION_H
#define WIDGETPLUGINCOLLECTION_H

#include <QtDesigner>
#include <QtCore>

/**
 * @brief Qt Designer plugin collection exposing KanoopGuiQt custom widgets.
 *
 * WidgetPluginCollection implements QDesignerCustomWidgetCollectionInterface so
 * that all custom KanoopGuiQt widgets are registered with Qt Designer as a
 * single plugin.  The list of widgets is populated in the constructor.
 */
class WidgetPluginCollection : public QObject,
                               public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
    /**
     * @brief Construct and populate the list of custom widget descriptors.
     * @param parent Optional QObject parent
     */
    explicit WidgetPluginCollection(QObject *parent = nullptr);

    /**
     * @brief Return all custom widget descriptors in this collection.
     * @return List of QDesignerCustomWidgetInterface pointers
     */
    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const override;

private:
    QList<QDesignerCustomWidgetInterface*> _widgets;
};

#endif // WIDGETPLUGINCOLLECTION_H
