#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT Application : public QApplication,
                    public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit Application(int& argc, char *argv[], const QString& applicationName);

signals:
    void preferencesChanged();
};

#endif // APPLICATION_H
