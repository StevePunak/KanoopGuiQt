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
#include <QSettings>
#include <QWidget>
#include <Kanoop/gui/libkanoopgui.h>

class QHeaderView;
class QSplitter;
class AbstractItemModel;
class TreeViewBase;
class LIBKANOOPGUI_EXPORT GuiSettings
{
public:
    GuiSettings();

    // Widget settings
    void setLastWindowPosition(const QWidget* widget, const QPoint& pos) { _settings.setValue(makeKey(KEY_LAST_WIDGET_POS, widget->objectName()), pos); }
    QPoint getLastWindowPosition(const QWidget* widget, const QSize& defaultSize = QSize()) const;

    void setLastWindowSize(const QWidget* widget, const QSize& size) { _settings.setValue(makeKey(KEY_LAST_WIDGET_SIZE, widget->objectName()), size); }
    QSize getLastWindowSize(const QWidget* widget, const QSize& defaultSize = QSize());

    void saveLastSplitterState(QSplitter* splitter);
    void restoreLastSplitterState(QSplitter* splitter);

    void saveLastHeaderState(QHeaderView* header);
    void restoreLastHeaderState(QHeaderView* header);

    void saveLastHeaderState(QHeaderView* header, AbstractItemModel* model);
    void restoreLastHeaderState(QHeaderView* header, AbstractItemModel* model);

    void saveTreeViewState(TreeViewBase* treeView);
    void restoreTreeViewState(TreeViewBase* treeView);

    void setStringValue(const QString& key, const QString& value) { _settings.setValue(key, value); }
    QString getStringValue(const QString& key) const { return _settings.value(key).toString(); }

    void setVariantValue(const QString& key, const QVariant& value) { _settings.setValue(key, value); }
    QVariant getVariantValue(const QString& key) const { return _settings.value(key); }

    void setByteArrayValue(const QString& key, const QByteArray& value) { _settings.setValue(key, value); }
    QByteArray getByteArrayValue(const QString& key) const { return _settings.value(key).toByteArray(); }

    int maxRecentFiles() const { return _maxRecentFiles; }
    void setMaxRecentFiles(int value) { _maxRecentFiles = value; }

    QString lastDirectory(const QString& extension) const { return _settings.value(makeFileTypeKey(KEY_LAST_DIRECTORY, extension)).toString(); }
    QString lastDirectory(int fileType) const { return _settings.value(makeFileTypeKey(KEY_LAST_DIRECTORY, fileType)).toString(); }
    void saveLastDirectory(const QString& extension, const QString& value) { _settings.setValue(makeFileTypeKey(KEY_LAST_DIRECTORY, extension), value); }
    void saveLastDirectory(int fileType, const QString& value) { _settings.setValue(makeFileTypeKey(KEY_LAST_DIRECTORY, fileType), value); }

    int fontSize() const { return _settings.value(makeStandardKey(KEY_FONT_SIZE)).toInt(); }
    void setFontSize(int value) { _settings.setValue(makeStandardKey(KEY_FONT_SIZE), value); }

    static GuiSettings* globalInstance();
    static void setGlobalInstance(GuiSettings* value) { _globalInstance = value; }

protected:
    static QString makeStandardKey(const QString& key) { return QString("%1/%2").arg(KEY_APP).arg(key); }
    static QString makeKey(const QString& key, const QString& subKey) { return QString("%1/%2").arg(key).arg(subKey); }
    static QString makeObjectKey(const QObject* object);
    static QString makeFileTypeKey(const QString& key, const QString& extension);
    static QString makeFileTypeKey(const QString& key, int fileType);
    static QString makeCompoundObjectKey(const QString& key, const QObject* object);

    virtual void ensureValidDefaults();

    QSettings _settings;

private:
    static QStringList uuidListToStringList(const QList<QUuid>& uuids);
    static QList<QUuid> stringListToUuidList(const QStringList& values);

    int _maxRecentFiles;

    static GuiSettings* _globalInstance;

    static const QString KEY_APP;
    static const QString KEY_FONT_SIZE;
    static const QString KEY_HEADER_STATE_HORIZ;
    static const QString KEY_HEADER_STATE_VERT;
    static const QString KEY_LAST_DIRECTORY;
    static const QString KEY_LAST_WIDGET_POS;
    static const QString KEY_LAST_WIDGET_SIZE;
    static const QString KEY_MODEL_HEADER_STATE_HORIZ;
    static const QString KEY_MODEL_HEADER_STATE_VERT;
    static const QString KEY_SPLITTER_STATE_HORIZ;
    static const QString KEY_SPLITTER_STATE_VERT;
    static const QString KEY_TREEVIEW_STATE;
};

#endif // GUISETTINGS_H
