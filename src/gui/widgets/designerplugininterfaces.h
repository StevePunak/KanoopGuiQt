#ifndef DESIGNERPLUGININTERFACES_H
#define DESIGNERPLUGININTERFACES_H

#include <QtUiPlugin>

class PlayPauseButtonDesignerPlugin : public QObject,
                                      public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    PlayPauseButtonDesignerPlugin(QObject* parent = nullptr) :
        QObject(parent) {}

    virtual QString name() const override { return "PlayPauseButton"; }
    virtual QString group() const override { return "Kanoop"; }
    virtual QString toolTip() const override { return "Play Pause Button"; }
    virtual QString whatsThis() const override { return "Play Pause Button"; }
    virtual QString includeFile() const override { return "Kanoop/gui/widgets/playpausebutton.h"; }
    virtual QIcon icon() const override;
    virtual bool isContainer() const override { return false; }
    virtual QWidget* createWidget(QWidget* parent) override;
};

#endif // DESIGNERPLUGININTERFACES_H
