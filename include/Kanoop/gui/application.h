#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QApplication subclass that adds logging support and a preferences-changed signal.
 *
 * Use this as the top-level application object in KanoopGuiQt-based applications.
 * Connect to preferencesChanged() to have widgets update when application preferences
 * are modified.
 */
class LIBKANOOPGUI_EXPORT Application : public QApplication,
                    public LoggingBaseClass
{
    Q_OBJECT
public:
    /**
     * @brief Construct the application object.
     * @param argc Reference to the command-line argument count (from main)
     * @param argv Command-line argument vector (from main)
     * @param applicationName Application name string used for settings and window titles
     */
    explicit Application(int& argc, char *argv[], const QString& applicationName);

signals:
    /** @brief Emitted when the user changes application preferences. */
    void preferencesChanged();
};

#endif // APPLICATION_H
