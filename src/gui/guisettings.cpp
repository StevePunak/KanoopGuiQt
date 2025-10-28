/******************************************************************************************
**
** guisettings.cpp
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#include "abstracttablemodel.h"
#include "guisettings.h"
#include "headerstate.h"
#include "treeviewbase.h"
#include <QApplication>
#include <QCoreApplication>
#include <QHeaderView>
#include <QMdiSubWindow>
#include <QMutex>
#include <QScreen>
#include <QSplitter>
#include <QTableView>
#include <QUuid>
#include <Kanoop/log.h>
#include <Kanoop/geometry/rectangle.h>
#include <Kanoop/geometry/size.h>

const QString GuiSettings::KEY_APP                          = "app";
const QString GuiSettings::KEY_FONT_SIZE                    = "font_size";
const QString GuiSettings::KEY_HEADER_STATE_HORIZ           = "header_state_h";
const QString GuiSettings::KEY_HEADER_STATE_VERT            = "header_state_v";
const QString GuiSettings::KEY_LAST_DIRECTORY               = "last_dir";
const QString GuiSettings::KEY_LAST_WIDGET_POS              = "widget_pos";
const QString GuiSettings::KEY_LAST_WIDGET_SIZE             = "widget_size";
const QString GuiSettings::KEY_MODEL_HEADER_STATE_HORIZ     = "model_header_state_h";
const QString GuiSettings::KEY_MODEL_HEADER_STATE_VERT      = "model_header_state_v";
const QString GuiSettings::KEY_SPLITTER_STATE_HORIZ         = "splitter_state_h";
const QString GuiSettings::KEY_SPLITTER_STATE_VERT          = "splitter_state_v";
const QString GuiSettings::KEY_TREEVIEW_STATE               = "treeview_state";

GuiSettings* GuiSettings::_globalInstance                   = nullptr;

GuiSettings::GuiSettings() :
    _settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::instance()->organizationName(), QCoreApplication::instance()->applicationName()),
    _maxRecentFiles(10)
{
}

QPoint GuiSettings::getLastWindowPosition(QWidget* widget, const QSize &defaultSize) const
{
    QPoint result;
    QString key = makeKey(KEY_LAST_WIDGET_POS, widget->objectName());
    if(_settings.contains(key)) {
        result = _settings.value(key).toPoint();
    }
    else {
        QWidget* parent = widget->parentWidget();

        bool isMdiSubWindow = qobject_cast<QMdiSubWindow*>(parent) != nullptr || qobject_cast<QMdiSubWindow*>(widget) != nullptr;
        if(isMdiSubWindow == false) {
            // There was no last size. Center the widget on the primary screen
            QScreen* screen = QApplication::primaryScreen();
            Rectangle screenGeometry = screen->geometry();
            Size widgetSize = defaultSize.isEmpty() ? widget->geometry().size() : defaultSize;
            result = QPoint(screenGeometry.centerPoint().x() - (widgetSize.width() / 2),
                            screenGeometry.centerPoint().y() - (widgetSize.height() / 2));
        }
    }
    result.setX(std::max(result.x(), 0));
    result.setY(std::max(result.y(), 0));
    return result;
}

QSize GuiSettings::getLastWindowSize(const QWidget *widget, const QSize &defaultSize)
{
    QSize result;
    QString key = makeKey(KEY_LAST_WIDGET_SIZE, widget->objectName());
    if(_settings.contains(key)) {
        result = _settings.value(key).toSize();
    }
    else {
        result = defaultSize.isEmpty() ? QSize(500, 500) : defaultSize;
        setLastWindowSize(widget, result);
    }
    return result;
}

bool GuiSettings::widgetHasPersistentGeometry(const QWidget* widget) const
{
    QString key = makeKey(KEY_LAST_WIDGET_SIZE, widget->objectName());
    return _settings.contains(key);
}

void GuiSettings::saveLastSplitterState(QSplitter *splitter)
{
    QString key = makeCompoundObjectKey(splitter->orientation() == Qt::Vertical
                                        ? KEY_SPLITTER_STATE_VERT
                                        : KEY_SPLITTER_STATE_HORIZ, splitter);
    _settings.setValue(key, splitter->saveState());
}

void GuiSettings::restoreLastSplitterState(QSplitter *splitter)
{
    QString key = makeCompoundObjectKey(splitter->orientation() == Qt::Vertical
                                        ? KEY_SPLITTER_STATE_VERT
                                        : KEY_SPLITTER_STATE_HORIZ, splitter);

    QByteArray savedState = splitter->saveState();
    QVariant value = _settings.value(key);
    QByteArray splitterState = value.toByteArray();
    if(splitterState.isEmpty() == false) {
        // if the count has changed, we should not restore the state
        int tempCount = splitter->count();
        splitter->restoreState(splitterState);
        if(tempCount != splitter->count()) {
            Log::logText(LVL_DEBUG, "Splitter count changed... will not restore it");
            splitter->restoreState(savedState);
        }
    }
}

void GuiSettings::saveLastHeaderState(QHeaderView *header)
{
    if(header->orientation() == Qt::Vertical) {
        _settings.setValue(makeKey(KEY_HEADER_STATE_VERT, makeObjectKey(header->parent())), header->saveState());
    }
    else {
        QString key = makeKey(KEY_HEADER_STATE_HORIZ, makeObjectKey(header->parent()));
        QByteArray headerState = header->saveState();
        _settings.setValue(key, headerState);
    }
}

void GuiSettings::restoreLastHeaderState(QHeaderView *header)
{
    QByteArray headerState;
    if(header->orientation() == Qt::Vertical) {
        headerState = _settings.value(makeKey(KEY_HEADER_STATE_VERT, makeObjectKey(header->parent()))).toByteArray();
    }
    else {
        QString key = makeKey(KEY_HEADER_STATE_HORIZ, makeObjectKey(header->parent()));
        headerState = _settings.value(key).toByteArray();
    }
    if(headerState.isEmpty() == false) {
        // Save the state. If the count of items has changed, we can't restore it
        int headerCount = header->count();
        QByteArray savedState = header->saveState();
        header->restoreState(headerState);
        if(header->count() != headerCount) {
            Log::logText(LVL_DEBUG, "Header count has changed... we will not restore it");
            header->restoreState(savedState);
        }
    }
}

void GuiSettings::saveLastHeaderState(QHeaderView *header, AbstractItemModel *model)
{
    if(model == nullptr) {
        return;
    }
    TableHeader::List headers = model->columnHeaders();
    HeaderState headerState;
    for(int section = 0;section < headers.count();section++) {
        TableHeader tableHeader = headers.at(section);
        headerState.addSection(section, tableHeader.text(), header->sectionSize(section), tableHeader.isVisible());
    }
    QByteArray jsonHeaderState = headerState.serializeToJson();
    if(header->orientation() == Qt::Vertical) {
        _settings.setValue(makeKey(KEY_MODEL_HEADER_STATE_VERT, makeObjectKey(header->parent())), jsonHeaderState);
    }
    else {
        _settings.setValue(makeKey(KEY_MODEL_HEADER_STATE_HORIZ, makeObjectKey(header->parent())), jsonHeaderState);
    }
}

void GuiSettings::restoreLastHeaderState(QHeaderView *header, AbstractItemModel *model)
{
    HeaderState headerState;
    QByteArray jsonState;
    QString key = header->orientation() == Qt::Vertical
                      ? makeKey(KEY_MODEL_HEADER_STATE_VERT, makeObjectKey(header->parent()))
                      : makeKey(KEY_MODEL_HEADER_STATE_HORIZ, makeObjectKey(header->parent()));
    jsonState = _settings.value(key).toByteArray();
    headerState.deserializeFromJson(jsonState);

    TableHeader::List headers = model->columnHeaders();
    for(int section = 0;section < headers.count();section++) {
        TableHeader tableHeader = headers.at(section);
        QString sectionText = tableHeader.text();
        HeaderState::SectionState sectionState = headerState.getSection(section);
        if(sectionState.isValid() && sectionState.text() == sectionText) {
            header->resizeSection(section, sectionState.size());
            QTableView* view = qobject_cast<QTableView*>(header->parent());
            if(view != nullptr) {
                if(sectionState.isVisible() == false) {
                    Log::logText(LVL_DEBUG, QString("Section %1 of %2 not visible").arg(section).arg(view->objectName()));
                }
                view->setColumnHidden(section, sectionState.isVisible() == false);
            }
        }
    }
}

void GuiSettings::saveTreeViewState(TreeViewBase *treeView)
{
    QByteArray state = treeView->saveState();
    _settings.setValue(makeKey(KEY_TREEVIEW_STATE, treeView->objectName()), state);
}

void GuiSettings::restoreTreeViewState(TreeViewBase *treeView)
{
    QByteArray state = _settings.value(makeKey(KEY_TREEVIEW_STATE, treeView->objectName())).toByteArray();
    treeView->restoreState(state);
}

GuiSettings *GuiSettings::globalInstance()
{
    QMutex _lock;
    _lock.lock();
    if(_globalInstance == nullptr) {
        _globalInstance = new GuiSettings;
    }
    _lock.unlock();
    return _globalInstance;
}

QString GuiSettings::makeObjectKey(const QObject *object)
{
    QString result = object->objectName();
    if(object->parent() != nullptr) {
        result.prepend(object->parent()->objectName());
    }
    return result;
}

QString GuiSettings::makeFileTypeKey(const QString &key, const QString &extension)
{
    QString fixed = extension;
    fixed.removeIf([extension](const QChar& it) { return it.isPunct() || it.isSpace(); });
    return QString("%1/%2").arg(key, fixed);
}

QString GuiSettings::makeFileTypeKey(const QString& key, int fileType)
{
    return QString("%1/%2").arg(key, QString("type_%1").arg(fileType));
}

QString GuiSettings::makeCompoundObjectKey(const QString &key, const QObject *object)
{
    QString objectKey = makeKey(key, makeObjectKey(object));
    return objectKey;
}

void GuiSettings::ensureValidDefaults()
{
    if(fontSize() == 0) {
        setFontSize(QFont().pointSize());
    }
}

QStringList GuiSettings::uuidListToStringList(const QList<QUuid> &uuids)
{
    QStringList result;
    for(const QUuid& uuid : uuids) {
        result.append(uuid.toString(QUuid::WithoutBraces));
    }
    return result;
}

QList<QUuid> GuiSettings::stringListToUuidList(const QStringList &values)
{
    QList<QUuid> result;
    for(const QString& value : values) {
        result.append(QUuid::fromString(value));
    }
    return result;
}
