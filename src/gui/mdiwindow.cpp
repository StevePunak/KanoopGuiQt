#include "mdiwindow.h"

#include "mdisubwindow.h"

#include <QMdiArea>
#include <guisettings.h>

MdiWindow::MdiWindow(const QString& logPrefix, QWidget* parent) :
    MainWindowBase(logPrefix, parent)
{
}

MainWindowBase* MdiWindow::activeSubWindow()
{
    MainWindowBase* result = nullptr;
    if(mdiArea() != nullptr && mdiArea()->activeSubWindow() != nullptr) {
        result = qobject_cast<MainWindowBase*>(mdiArea()->activeSubWindow()->widget());
    }
    return result;
}

void MdiWindow::openSubWindow(MainWindowBase* window, int type)
{
    if(mdiArea() == nullptr) {
        logText(LVL_ERROR, "Can't add sub-window without setting MDI area first");
        return;
    }

    window->setType(type);

    MdiSubWindow* mdiSubWindow = new MdiSubWindow;
    mdiSubWindow->setObjectName(QString("%1-mdiSub").arg(window->objectName()));
    mdiSubWindow->setWidget(window);
    mdiSubWindow->setType(type);
    connect(mdiSubWindow, &MdiSubWindow::closing, this, &MdiWindow::onSubWindowClosing);
    mdiArea()->addSubWindow(mdiSubWindow);

    QPoint pos = GuiSettings::globalInstance()->getLastWindowPosition(mdiSubWindow, window->defaultSize());
    QSize size = GuiSettings::globalInstance()->getLastWindowSize(mdiSubWindow, window->defaultSize());
    mdiSubWindow->move(pos);
    mdiSubWindow->resize(size);

    mdiSubWindow->show();
    window->show();
}

void MdiWindow::closeSubWindows(int type)
{
    for(MdiSubWindow* window : findMdiSubWindows(type)) {
        window->close();
        delete window;
    }
}

MdiSubWindow* MdiWindow::findFirstMdiSubWindow(int type) const
{
    MdiSubWindow* result = nullptr;
    for(MdiSubWindow* window : findMdiSubWindows(type)) {
        result = window;
        break;
    }
    return result;
}

QList<MdiSubWindow*> MdiWindow::findMdiSubWindows(int type) const
{
    QList<MdiSubWindow*> result;
    for(MdiSubWindow* window : findChildren<MdiSubWindow*>()) {
        if(window->type() == type) {
            result.append(window);
        }
    }
    return result;
}

QMdiArea* MdiWindow::mdiArea()
{
    QMdiArea* result = qobject_cast<QMdiArea*>(centralWidget());
    return result;
}

void MdiWindow::onSubWindowClosing()
{
    MdiSubWindow* window = qobject_cast<MdiSubWindow*>(sender());
    if(window != nullptr) {
        subWindowClosing(window);
    }
}

