#ifndef LABELDESIGNERPLUGIN_H
#define LABELDESIGNERPLUGIN_H
#include <QtUiPlugin>

class LabelDesignerPlugin : public QObject,
                                      public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    LabelDesignerPlugin(QObject* parent = nullptr) :
        QObject(parent) {}

    virtual QString name() const override { return "Colored Label"; }
    virtual QString group() const override { return "Kanoop"; }
    virtual QString toolTip() const override { return "Colored Label"; }
    virtual QString whatsThis() const override { return "Colored Label"; }
    virtual QString includeFile() const override { return "Kanoop/gui/widgets/label.h"; }
    virtual QIcon icon() const override;
    virtual bool isContainer() const override { return false; }
    virtual QWidget* createWidget(QWidget* parent) override;
};

#endif // LABELDESIGNERPLUGIN_H
