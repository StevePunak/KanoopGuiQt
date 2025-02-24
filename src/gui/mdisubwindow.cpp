#include "mdisubwindow.h"

#include "guisettings.h"

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
    if(GuiSettings::globalInstance() != nullptr) {
        GuiSettings::globalInstance()->setLastWindowPosition(this, event->pos());
    }
    QMdiSubWindow::moveEvent(event);
}

void MdiSubWindow::resizeEvent(QResizeEvent* event)
{
    // Log::logText(LVL_DEBUG, QString("%1: resize to %2").arg(objectName()).arg(Size(event->size()).toString()));
    if(GuiSettings::globalInstance() != nullptr) {
        GuiSettings::globalInstance()->setLastWindowSize(this, event->size());
    }
    QMdiSubWindow::resizeEvent(event);
}

void MdiSubWindow::showEvent(QShowEvent* event)
{
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
