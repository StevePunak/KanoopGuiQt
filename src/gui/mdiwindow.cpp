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

MdiSubWindow* MdiWindow::openSubWindow(MainWindowBase* window, int type)
{
    if(mdiArea() == nullptr) {
        logText(LVL_ERROR, "Can't add sub-window without setting MDI area first");
        return nullptr;
    }

    window->setType(type);

    // load the existing of the same type
    QList<MdiSubWindow*> existing = findMdiSubWindows(type);

    // create the new window
    MdiSubWindow* mdiSubWindow = new MdiSubWindow;
    mdiSubWindow->setObjectName(QString("%1-mdiSub").arg(window->objectName()));
    mdiSubWindow->setWidget(window);
    mdiSubWindow->setType(type);
    connect(mdiSubWindow, &MdiSubWindow::closing, this, &MdiWindow::onSubWindowClosing);
    mdiArea()->addSubWindow(mdiSubWindow);

    QPoint pos = GuiSettings::globalInstance()->getLastWindowPosition(mdiSubWindow, window->defaultSize());
    QSize size = GuiSettings::globalInstance()->getLastWindowSize(mdiSubWindow, window->defaultSize());
    if(existing.count() > 0) {
        // position down and to the right a bit from the last existing
        static const int NewWindowOffset = 20;
        pos = QPoint(existing.last()->pos().x() + NewWindowOffset, existing.last()->pos().y() + NewWindowOffset);
    }
    if(window->persistPosition()) {
        mdiSubWindow->move(pos);
    }

    if(window->persistSize()) {
        mdiSubWindow->resize(size);
    }

    mdiSubWindow->show();
    window->show();

    return mdiSubWindow;
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

