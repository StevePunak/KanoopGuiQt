#ifndef COMPLEXWIDGET_H
#define COMPLEXWIDGET_H

#include <QWidget>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT ComplexWidget : public QWidget,
                      public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit ComplexWidget(QWidget *parent = nullptr);
    explicit ComplexWidget(const QString& loggingCategory, QWidget *parent = nullptr);

private:
    void commonInit();

protected:
    void initializeBase();
    void connectValidationSignals();

    virtual void validate() {}

private:
    void connectLineEditSignals();
    void connectComboBoxSignals();
    void connectRadioButtonSignals();
    void connectCheckBoxSignals();
    void connectSpinBoxSignals();

public slots:
    virtual void onPreferencesChanged();

private slots:
    virtual void onSplitterMoved();
    virtual void stringChanged(const QString&) { validate(); }
    virtual void intChanged(int) { validate(); }
    virtual void boolChanged(bool) { validate(); }
    virtual void voidChanged() { validate(); }

signals:

};

#endif // COMPLEXWIDGET_H
