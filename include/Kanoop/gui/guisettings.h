/******************************************************************************************
**
** guisettings.h
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#ifndef GUISETTINGS_H
#define GUISETTINGS_H
#include <QWidget>
#include <Kanoop/gui/libkanoopgui.h>
#include <Kanoop/utility/appsettings.h>

class QHeaderView;
class QSplitter;
class AbstractItemModel;
class TreeViewBase;

/**
 * @brief Persistent GUI settings storage for widget geometry, header states, and directories.
 *
 * GuiSettings extends AppSettings with typed accessors for common GUI state such as
 * window positions, sizes, splitter states, header view states, and tree view state.
 * Subclass to add application-specific settings.
 *
 * A process-wide singleton can be registered with setGlobalInstance() and retrieved
 * with globalInstance().
 */
class LIBKANOOPGUI_EXPORT GuiSettings : public AppSettings
{
    Q_OBJECT

public:
    /** @brief Construct a GuiSettings object backed by the default QSettings store. */
    GuiSettings();

    // Widget settings
    /**
     * @brief Persist the last position of a widget.
     * @param widget Widget whose position to save (key is based on objectName)
     * @param pos Position to save
     */
    void setLastWindowPosition(const QWidget* widget, const QPoint& pos) { _settings.setValue(makeKey(KEY_LAST_WIDGET_POS, widget->objectName()), pos); }

    /**
     * @brief Retrieve the last saved position of a widget.
     * @param widget Widget to look up
     * @param defaultSize Default size used to compute a centred position if no position is saved
     * @return Saved position, or a sensible default
     */
    QPoint getLastWindowPosition(QWidget* widget, const QSize& defaultSize = QSize()) const;

    /**
     * @brief Persist the last size of a widget.
     * @param widget Widget whose size to save
     * @param size Size to save
     */
    void setLastWindowSize(const QWidget* widget, const QSize& size) { _settings.setValue(makeKey(KEY_LAST_WIDGET_SIZE, widget->objectName()), size); }

    /**
     * @brief Retrieve the last saved size of a widget.
     * @param widget Widget to look up
     * @param defaultSize Size to return if nothing is saved
     * @return Saved size, or defaultSize
     */
    QSize getLastWindowSize(const QWidget* widget, const QSize& defaultSize = QSize());

    /**
     * @brief Return whether any geometry has been saved for a widget.
     * @param widget Widget to check
     * @return true if a saved position or size exists
     */
    bool widgetHasPersistentGeometry(const QWidget* widget) const;

    /**
     * @brief Save the current state of a QSplitter.
     * @param splitter Splitter whose state to save
     */
    void saveLastSplitterState(QSplitter* splitter);

    /**
     * @brief Restore a previously saved QSplitter state.
     * @param splitter Splitter to restore
     */
    void restoreLastSplitterState(QSplitter* splitter);

    /**
     * @brief Save the current state of a QHeaderView.
     * @param header Header view whose state to save
     */
    void saveLastHeaderState(QHeaderView* header);

    /**
     * @brief Restore a previously saved QHeaderView state.
     * @param header Header view to restore
     */
    void restoreLastHeaderState(QHeaderView* header);

    /**
     * @brief Save header state using the model's column definitions as keys.
     * @param header Header view whose state to save
     * @param model Model providing column metadata for keying
     */
    void saveLastHeaderState(QHeaderView* header, AbstractItemModel* model);

    /**
     * @brief Restore header state using the model's column definitions as keys.
     * @param header Header view to restore
     * @param model Model providing column metadata for keying
     */
    void restoreLastHeaderState(QHeaderView* header, AbstractItemModel* model);

    /**
     * @brief Save the expansion/selection state of a TreeViewBase.
     * @param treeView Tree view whose state to save
     */
    void saveTreeViewState(TreeViewBase* treeView);

    /**
     * @brief Restore the expansion/selection state of a TreeViewBase.
     * @param treeView Tree view to restore
     */
    void restoreTreeViewState(TreeViewBase* treeView);

    /**
     * @brief Return the persisted font size.
     * @return Font point size, or 0 if not set
     */
    int fontSize() const { return _settings.value(makeStandardKey(KEY_FONT_SIZE)).toInt(); }

    /**
     * @brief Persist the font size.
     * @param value Font point size
     */
    void setFontSize(int value) { _settings.setValue(makeStandardKey(KEY_FONT_SIZE), value); }

    /**
     * @brief Return the process-wide GuiSettings singleton.
     * @return Global GuiSettings instance, or nullptr if not set
     */
    static GuiSettings* globalInstance() { return static_cast<GuiSettings*>(AppSettings::globalInstance()); }

protected:
    /** @brief Override to ensure sane default values on first run. */
    virtual void ensureValidDefaults() override;

    /** @brief Settings key for the saved font size. */
    static const QString KEY_FONT_SIZE;
    /** @brief Settings key for the horizontal header state. */
    static const QString KEY_HEADER_STATE_HORIZ;
    /** @brief Settings key for the vertical header state. */
    static const QString KEY_HEADER_STATE_VERT;
    /** @brief Settings key for the last widget position. */
    static const QString KEY_LAST_WIDGET_POS;
    /** @brief Settings key for the last widget size. */
    static const QString KEY_LAST_WIDGET_SIZE;
    /** @brief Settings key for the horizontal model header state. */
    static const QString KEY_MODEL_HEADER_STATE_HORIZ;
    /** @brief Settings key for the vertical model header state. */
    static const QString KEY_MODEL_HEADER_STATE_VERT;
    /** @brief Settings key for the horizontal splitter state. */
    static const QString KEY_SPLITTER_STATE_HORIZ;
    /** @brief Settings key for the vertical splitter state. */
    static const QString KEY_SPLITTER_STATE_VERT;
    /** @brief Settings key for the tree view expansion state. */
    static const QString KEY_TREEVIEW_STATE;
};

#endif // GUISETTINGS_H
