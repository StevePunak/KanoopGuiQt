#include "application.h"

Application::Application(int& argc, char* argv[], const QString& applicationName) :
    QApplication(argc, argv),
    LoggingBaseClass(applicationName)
{
    setApplicationName(applicationName);
}
