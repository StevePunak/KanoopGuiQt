#include "mdisubwindow.h"

#include "guisettings.h"
#include "mainwindowbase.h"

#include <QMenu>
#include <QMoveEvent>
#include <Kanoop/log.h>
#include <Kanoop/geometry/point.h>
#include <Kanoop/geometry/size.h>

MdiSubWindow::MdiSubWindow(QWidget* parent) :
    QMdiSubWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
}

void MdiSubWindow::moveEvent(QMoveEvent* event)
{
    // Log::logText(LVL_DEBUG, QString("%1: move to %2").arg(objectName()).arg(Point(event->pos()).toString()));
    if(GuiSettings::globalInstance() != nullptr && _formLoadComplete && isMaximized() == false) {
        GuiSettings::globalInstance()->setLastWindowPosition(this, event->pos());
        const QString kindName = geometryKindObjectName();
        if(kindName.isEmpty() == false) {
            GuiSettings::globalInstance()->setLastWindowPosition(kindName, event->pos());
        }
    }
    QMdiSubWindow::moveEvent(event);
}

void MdiSubWindow::resizeEvent(QResizeEvent* event)
{
    // Log::logText(LVL_DEBUG, QString("%1: resize to %2").arg(objectName()).arg(Size(event->size()).toString()));
    if(GuiSettings::globalInstance() != nullptr && _formLoadComplete && isMaximized() == false) {
        GuiSettings::globalInstance()->setLastWindowSize(this, event->size());
        const QString kindName = geometryKindObjectName();
        if(kindName.isEmpty() == false) {
            GuiSettings::globalInstance()->setLastWindowSize(kindName, event->size());
        }
    }
    QMdiSubWindow::resizeEvent(event);
}

QString MdiSubWindow::geometryKindObjectName() const
{
    MainWindowBase* child = qobject_cast<MainWindowBase*>(widget());
    if(child == nullptr) {
        return QString();
    }
    return QString("%1%2").arg(child->geometryKindName(), MdiSubWindowSuffix);
}

void MdiSubWindow::showEvent(QShowEvent* event)
{
    // ⚠ The flag must not become true until first-show geometry has been dispatched. Qt delivers
    // Move and Resize to a hidden widget before the Show event, so the placement openSubWindow()
    // applied arrives while this is still false and is not stored. Setting it any earlier, in a
    // constructor or on a line above this one, stores that placement as a user's choice.
    if(!_formLoadComplete) {
        _formLoadComplete = true;
    }
    QMdiSubWindow::showEvent(event);
}

void MdiSubWindow::closeEvent(QCloseEvent* event)
{
    emit closing();
    QMdiSubWindow::closeEvent(event);
}

void MdiSubWindow::onPreferencesChanged()
{
    if(GuiSettings::globalInstance() != nullptr) {
        QFont newFont(font());
        int pointSize = GuiSettings::globalInstance()->fontSize();
        newFont.setPointSize(pointSize);
        setFont(newFont);

        QList<QMenu*> menus = findChildren<QMenu*>();
        for(QMenu* menu : menus) {
            menu->setFont(newFont);
        }
    }
}

#include "Kanoop/gui/moc_mdisubwindow.cpp"
