#ifndef CHECKBOXDESIGNERPLUGIN_H
#define CHECKBOXDESIGNERPLUGIN_H
#include <QtUiPlugin>

class CheckBoxDesignerPlugin : public QObject,
                               public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    CheckBoxDesignerPlugin(QObject* parent = nullptr) :
        QObject(parent) {}

    virtual QString name() const override { return "Check Box Widget"; }
    virtual QString group() const override { return "Kanoop"; }
    virtual QString toolTip() const override { return "Check Box Widget"; }
    virtual QString whatsThis() const override { return "Check Box Widget"; }
    virtual QString includeFile() const override { return "Kanoop/gui/widgets/xxxcheckbox.h"; }
    virtual QIcon icon() const override;
    virtual bool isContainer() const override { return false; }
    virtual QWidget* createWidget(QWidget* parent) override;
};

#endif // CHECKBOXDESIGNERPLUGIN_H
