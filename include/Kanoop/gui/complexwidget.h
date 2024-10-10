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

public slots:
    virtual void onPreferencesChanged();

private slots:
    void onSplitterMoved();

signals:

};

#endif // COMPLEXWIDGET_H
