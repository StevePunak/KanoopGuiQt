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
#include <QObject>
#include <QSettings>
#include <QWidget>
#include <Kanoop/gui/libkanoopgui.h>

class QHeaderView;
class QSplitter;
class AbstractItemModel;
class TreeViewBase;

/**
 * @brief Persistent GUI settings storage for widget geometry, header states, and directories.
 *
 * GuiSettings wraps QSettings to provide typed, named accessors for common GUI state
 * such as window positions, sizes, splitter states, header view states, and last-used
 * directories.  Subclass to add application-specific settings.
 *
 * A process-wide singleton can be registered with setGlobalInstance() and retrieved
 * with globalInstance().
 */
class LIBKANOOPGUI_EXPORT GuiSettings : public QObject
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
     * @brief Persist an arbitrary string value.
     * @param key Settings key
     * @param value String to save
     */
    void setStringValue(const QString& key, const QString& value) { _settings.setValue(key, value); }

    /**
     * @brief Retrieve an arbitrary string value.
     * @param key Settings key
     * @return Saved string, or empty string if not found
     */
    QString getStringValue(const QString& key) const { return _settings.value(key).toString(); }

    /**
     * @brief Persist an arbitrary QVariant value.
     * @param key Settings key
     * @param value Variant to save
     */
    void setVariantValue(const QString& key, const QVariant& value) { _settings.setValue(key, value); }

    /**
     * @brief Retrieve an arbitrary QVariant value.
     * @param key Settings key
     * @return Saved variant, or invalid QVariant if not found
     */
    QVariant getVariantValue(const QString& key) const { return _settings.value(key); }

    /**
     * @brief Persist an arbitrary byte array value.
     * @param key Settings key
     * @param value Byte array to save
     */
    void setByteArrayValue(const QString& key, const QByteArray& value) { _settings.setValue(key, value); }

    /**
     * @brief Retrieve an arbitrary byte array value.
     * @param key Settings key
     * @return Saved byte array, or empty array if not found
     */
    QByteArray getByteArrayValue(const QString& key) const { return _settings.value(key).toByteArray(); }

    /**
     * @brief Return the maximum number of recent files to track.
     * @return Maximum recent file count
     */
    int maxRecentFiles() const { return _maxRecentFiles; }

    /**
     * @brief Set the maximum number of recent files to track.
     * @param value Maximum recent file count
     */
    void setMaxRecentFiles(int value) { _maxRecentFiles = value; }

    /**
     * @brief Return the last directory used for a given file extension.
     * @param extension File extension (without leading dot)
     * @return Last directory path string
     */
    QString lastDirectory(const QString& extension) const { return _settings.value(makeFileTypeKey(KEY_LAST_DIRECTORY, extension)).toString(); }

    /**
     * @brief Return the last directory used for a given file-type integer.
     * @param fileType Application-defined file type integer
     * @return Last directory path string
     */
    QString lastDirectory(int fileType) const { return _settings.value(makeFileTypeKey(KEY_LAST_DIRECTORY, fileType)).toString(); }

    /**
     * @brief Save the last directory used for a given file extension.
     * @param extension File extension (without leading dot)
     * @param value Directory path to save
     */
    virtual void saveLastDirectory(const QString& extension, const QString& value) { _settings.setValue(makeFileTypeKey(KEY_LAST_DIRECTORY, extension), value); }

    /**
     * @brief Save the last directory used for a given file-type integer.
     * @param fileType Application-defined file type integer
     * @param value Directory path to save
     */
    virtual void saveLastDirectory(int fileType, const QString& value) { _settings.setValue(makeFileTypeKey(KEY_LAST_DIRECTORY, fileType), value); }

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
    static GuiSettings* globalInstance();

    /**
     * @brief Set the process-wide GuiSettings singleton.
     * @param value Instance to register as global
     */
    static void setGlobalInstance(GuiSettings* value) { _globalInstance = value; }

signals:
    /** @brief Emitted when any setting value changes. */
    void settingsChanged();

protected:
    /** @brief Build a settings key prefixed with the application key. */
    static QString makeStandardKey(const QString& key) { return QString("%1/%2").arg(KEY_APP).arg(key); }
    /** @brief Build a compound settings key from two parts. */
    static QString makeKey(const QString& key, const QString& subKey) { return QString("%1/%2").arg(key).arg(subKey); }
    /** @brief Build a settings key based on a QObject's identity. */
    static QString makeObjectKey(const QObject* object);
    /** @brief Build a settings key based on a file extension. */
    static QString makeFileTypeKey(const QString& key, const QString& extension);
    /** @brief Build a settings key based on a file-type integer. */
    static QString makeFileTypeKey(const QString& key, int fileType);
    /** @brief Build a compound settings key from a base key and a QObject. */
    static QString makeCompoundObjectKey(const QString& key, const QObject* object);

    /** @brief Override to ensure sane default values on first run. */
    virtual void ensureValidDefaults();

    /** @brief Underlying QSettings storage. */
    QSettings _settings;

    /** @brief Settings key for the application group. */
    static const QString KEY_APP;
    /** @brief Settings key for the saved font size. */
    static const QString KEY_FONT_SIZE;
    /** @brief Settings key for the horizontal header state. */
    static const QString KEY_HEADER_STATE_HORIZ;
    /** @brief Settings key for the vertical header state. */
    static const QString KEY_HEADER_STATE_VERT;
    /** @brief Settings key for the last-used directory. */
    static const QString KEY_LAST_DIRECTORY;
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

private:
    /** @brief Convert a list of UUIDs to a list of strings for persistence. */
    static QStringList uuidListToStringList(const QList<QUuid>& uuids);
    /** @brief Convert a list of strings back to UUIDs. */
    static QList<QUuid> stringListToUuidList(const QStringList& values);

    int _maxRecentFiles;

    static GuiSettings* _globalInstance;
};

#endif // GUISETTINGS_H
