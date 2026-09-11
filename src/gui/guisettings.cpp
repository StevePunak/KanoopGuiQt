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
#include <QFont>
#include <QHeaderView>
#include <QMdiSubWindow>
#include <QScreen>
#include <QSplitter>
#include <QTableView>
#include <Kanoop/log.h>
#include <Kanoop/geometry/rectangle.h>
#include <Kanoop/geometry/size.h>

const QString GuiSettings::KEY_FONT_SIZE                    = "font_size";
const QString GuiSettings::KEY_HEADER_STATE_HORIZ           = "header_state_h";
const QString GuiSettings::KEY_HEADER_STATE_VERT            = "header_state_v";
const QString GuiSettings::KEY_LAST_WIDGET_POS              = "widget_pos";
const QString GuiSettings::KEY_LAST_WIDGET_SIZE             = "widget_size";
const QString GuiSettings::KEY_MODEL_HEADER_STATE_HORIZ     = "model_header_state_h";
const QString GuiSettings::KEY_MODEL_HEADER_STATE_VERT      = "model_header_state_v";
const QString GuiSettings::KEY_SPLITTER_STATE_HORIZ         = "splitter_state_h";
const QString GuiSettings::KEY_SPLITTER_STATE_VERT          = "splitter_state_v";
const QString GuiSettings::KEY_TREEVIEW_STATE               = "treeview_state";

GuiSettings::GuiSettings() :
    AppSettings()
{
}

QPoint GuiSettings::getLastWindowPosition(QWidget* widget, const QSize &defaultSize) const
{
    QPoint result;
    QWidget* parent = widget->parentWidget();
    bool isMdiSubWindow = qobject_cast<QMdiSubWindow*>(parent) != nullptr || qobject_cast<QMdiSubWindow*>(widget) != nullptr;

    QString key = makeKey(KEY_LAST_WIDGET_POS, widget->objectName());
    if(_settings.contains(key)) {
        result = _settings.value(key).toPoint();
    }
    else if(isMdiSubWindow == false) {
        // There was no last size. Center the widget on the primary screen
        QScreen* screen = QApplication::primaryScreen();
        Rectangle screenGeometry = screen->geometry();
        Size widgetSize = defaultSize.isEmpty() ? widget->geometry().size() : defaultSize;
        result = QPoint(screenGeometry.centerPoint().x() - (widgetSize.width() / 2),
                        screenGeometry.centerPoint().y() - (widgetSize.height() / 2));
    }

    // ⚠ The clamp holds only for an MDI subwindow, positioned in its area's client coordinates
    // where a negative value is off the viewport. A top-level window is positioned on the desktop,
    // where a monitor left of or above the primary legitimately has negative coordinates; clamping
    // those drags the window onto the primary screen and discards the restore point.
    // MainWindowBase::boundToScreen and Dialog::showEvent validate desktop positions instead.
    if(isMdiSubWindow == true) {
        result.setX(std::max(result.x(), 0));
        result.setY(std::max(result.y(), 0));
    }
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

bool GuiSettings::widgetHasPersistentPosition(const QWidget* widget) const
{
    QString key = makeKey(KEY_LAST_WIDGET_POS, widget->objectName());
    return _settings.contains(key);
}

bool GuiSettings::tryGetLastWindowPosition(const QString& objectName, QPoint& result) const
{
    QString key = makeKey(KEY_LAST_WIDGET_POS, objectName);
    if(_settings.contains(key) == false) {
        return false;
    }
    result = _settings.value(key).toPoint();
    return true;
}

bool GuiSettings::tryGetLastWindowSize(const QString& objectName, QSize& result) const
{
    QString key = makeKey(KEY_LAST_WIDGET_SIZE, objectName);
    if(_settings.contains(key) == false) {
        return false;
    }
    result = _settings.value(key).toSize();
    return true;
}

QStringList GuiSettings::persistentGeometryNames() const
{
    const QString positionPrefix = QString("%1/").arg(KEY_LAST_WIDGET_POS);
    const QString sizePrefix = QString("%1/").arg(KEY_LAST_WIDGET_SIZE);

    QStringList result;
    for(const QString& key : _settings.allKeys()) {
        QString objectName;
        if(key.startsWith(positionPrefix)) {
            objectName = key.mid(positionPrefix.length());
        }
        else if(key.startsWith(sizePrefix)) {
            objectName = key.mid(sizePrefix.length());
        }
        if(objectName.isEmpty() == false && result.contains(objectName) == false) {
            result.append(objectName);
        }
    }
    return result;
}

void GuiSettings::removePersistentGeometry(const QString& objectName)
{
    // ⚠ An empty name makes the key "widget_pos/", which QSettings collapses onto the group name
    // "widget_pos" -- removing that drops every stored position in the application.
    if(objectName.isEmpty()) {
        return;
    }
    removeKey(makeKey(KEY_LAST_WIDGET_POS, objectName));
    removeKey(makeKey(KEY_LAST_WIDGET_SIZE, objectName));
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
        headerState.addSection(section, tableHeader.text(), header->sectionSize(section), tableHeader.isVisible(), header->visualIndex(section));
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
    // visualIndex -> logical section. Built only if every section matches the saved state.
    QMap<int, int> logicalAtVisual;
    bool orderComplete = true;
    for(int section = 0;section < headers.count();section++) {
        TableHeader tableHeader = headers.at(section);
        QString sectionText = tableHeader.text();
        HeaderState::SectionState sectionState = headerState.getSection(section);
        if(sectionState.isValid() && sectionState.text() == sectionText) {
            // A stretch section's width comes from the layout, so a persisted width does not apply to it.
            bool stretchSection = header->sectionResizeMode(section) == QHeaderView::Stretch ||
                                  (header->stretchLastSection() && section == header->count() - 1);
            if(stretchSection == false) {
                header->resizeSection(section, sectionState.size());
            }
            QTableView* view = qobject_cast<QTableView*>(header->parent());
            if(view != nullptr) {
                if(sectionState.isVisible() == false) {
                    Log::logText(LVL_DEBUG, QString("Section %1 of %2 not visible").arg(section).arg(view->objectName()));
                }
                view->setColumnHidden(section, sectionState.isVisible() == false);
            }
            logicalAtVisual.insert(sectionState.visualIndex(), section);
        }
        else {
            orderComplete = false;
        }
    }

    // Restore visual (drag) order only when the saved state fully describes the current
    // column set. Block header signals so the moves do not re-enter the save slot.
    if(orderComplete && logicalAtVisual.count() == headers.count()) {
        QSignalBlocker blocker(header);
        for(int targetVisual = 0;targetVisual < headers.count();targetVisual++) {
            int logical = logicalAtVisual.value(targetVisual, -1);
            if(logical < 0) {
                continue;
            }
            int currentVisual = header->visualIndex(logical);
            if(currentVisual >= 0 && currentVisual != targetVisual) {
                header->moveSection(currentVisual, targetVisual);
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

void GuiSettings::ensureValidDefaults()
{
    if(fontSize() == 0) {
        setFontSize(QFont().pointSize());
    }
}

#include "Kanoop/gui/moc_guisettings.cpp"
