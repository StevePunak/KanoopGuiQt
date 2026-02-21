/******************************************************************************************
**
** mainwindowbase.h
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#ifndef MAINWINDOWBASE_H
#define MAINWINDOWBASE_H

#include <QMainWindow>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>
#include <Kanoop/timespan.h>
#include <Kanoop/gui/widgets/statusbar.h>

class QMdiArea;

/**
 * @brief QMainWindow subclass providing logging, status bar helpers, and geometry persistence.
 *
 * MainWindowBase wires a StatusBar, persists window position/size across sessions, and
 * exposes slots for showing plain or animated status messages. Call initializeBase()
 * from the subclass constructor after building the UI.
 */
class LIBKANOOPGUI_EXPORT MainWindowBase : public QMainWindow,
                                           public LoggingBaseClass
{
    Q_OBJECT
public:
    /**
     * @brief Construct with a logging category and optional parent.
     * @param loggingCategory Category name used for log output
     * @param parent Optional QWidget parent
     */
    explicit MainWindowBase(const QString& loggingCategory, QWidget *parent = nullptr);

    /**
     * @brief Return whether the window persists its position between sessions.
     * @return true if position persistence is enabled
     */
    bool persistPosition() const { return _persistPosition; }

    /**
     * @brief Enable or disable position persistence.
     * @param value true to persist position
     */
    void setPersistPosition(bool value) { _persistPosition = value; }

    /**
     * @brief Return whether the window persists its size between sessions.
     * @return true if size persistence is enabled
     */
    bool persistSize() const { return _persistSize; }

    /**
     * @brief Enable or disable size persistence.
     * @param value true to persist size
     */
    void setPersistSize(bool value) { _persistSize = value; }

    /**
     * @brief Return the application-defined window type integer.
     * @return Window type value
     */
    int type() const { return _type; }

    /**
     * @brief Set the application-defined window type integer.
     * @param value Window type value
     */
    void setType(int value) { _type = value; }

    /**
     * @brief Return the default window size.
     * @return Default QSize
     */
    QSize defaultSize() const { return _defaultSize; }

    /**
     * @brief Set the default window size.
     * @param value Default QSize
     */
    void setDefaultSize(const QSize& value) { _defaultSize = value; }

    /**
     * @brief Set the default window size by width and height.
     * @param width Default width in pixels
     * @param height Default height in pixels
     */
    void setDefaultSize(int width, int height) { _defaultSize = QSize(width, height); }

public slots:
    /**
     * @brief Show a coloured status message with an optional timeout.
     * @param text Message text
     * @param textColor Foreground colour for the message
     * @param timeout Duration before the message is cleared (default: no timeout)
     */
    void showStatusBarMessage(const QString &text, const QColor& textColor, const TimeSpan& timeout = TimeSpan());

    /**
     * @brief Show a default-coloured status message with an optional timeout.
     * @param text Message text
     * @param timeout Duration before the message is cleared (default: no timeout)
     */
    void showStatusBarMessage(const QString& text, const TimeSpan& timeout = TimeSpan());

    /**
     * @brief Show an animated progress message in the status bar.
     * @param text Message text (dots are appended periodically)
     * @param textColor Foreground colour (default: palette default)
     */
    void showStatusBarAnimatedProgressMessage(const QString &text, const QColor& textColor = QColor());

    /** @brief Stop any active status bar animation. */
    void stopStatusBarAnimation();

public:
    /**
     * @brief Return the parent QMdiArea if this window is hosted in one.
     * @return Pointer to the parent MdiArea, or nullptr
     */
    QMdiArea* parentMdiArea();

    /**
     * @brief Return the built-in StatusBar widget.
     * @return Pointer to the StatusBar
     */
    StatusBar* statusBar();

    /**
     * @brief Return whether the form has finished loading.
     * @return true if form load is complete
     */
    bool formLoadComplete() const { return _formLoadComplete; }

    /**
     * @brief Return whether the form failed to load.
     * @return true if form load failed
     */
    bool formLoadFailed() const { return _formLoadFailed; }

protected:
    /** @brief Perform base-class initialization; call from the subclass constructor. */
    void initializeBase();

    /**
     * @brief Set the form load completion flag.
     * @param value true when form load is complete
     */
    void setFormLoadComplete(bool value) { _formLoadComplete = value; }

    /**
     * @brief Set the form load failure flag.
     * @param value true if form load failed
     */
    void setFormLoadFailed(bool value) { _formLoadFailed = value; }

    /** @brief Persist position on move. */
    virtual void moveEvent(QMoveEvent *event) override;
    /** @brief Persist size on resize. */
    virtual void resizeEvent(QResizeEvent *event) override;
    /** @brief Restore geometry and complete form load on first show. */
    virtual void showEvent(QShowEvent *event) override;

private:
    int _type = 0;
    bool _formLoadComplete = false;
    bool _formLoadFailed = false;
    bool _persistPosition = true;
    bool _persistSize = true;
    QSize _defaultSize;
    StatusBar* _statusBar = nullptr;

signals:

public slots:
    /** @brief Called when application preferences change; override to react. */
    virtual void onPreferencesChanged();

private slots:
    /** @brief Persist splitter state when moved. */
    void onSpliltterMoved();

};

#endif // MAINWINDOWBASE_H
