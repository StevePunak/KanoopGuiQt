#include "mdiwindow.h"

#include "mdisubwindow.h"

#include <QMdiArea>
#include "guisettings.h"

#include <algorithm>

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
    mdiSubWindow->setObjectName(QString("%1%2").arg(window->objectName(), MdiSubWindow::MdiSubWindowSuffix));
    mdiSubWindow->setWidget(window);
    mdiSubWindow->setType(type);
    connect(mdiSubWindow, &MdiSubWindow::closing, this, &MdiWindow::onSubWindowClosing);
    mdiArea()->addSubWindow(mdiSubWindow);

    // ⚠ The CASCADE decision is made on the position key. A window with a stored position but
    // no stored size -- moved but never resized -- reads as never-seen when the size key is
    // asked, and gets cascaded over the top of itself. hasStoredSize below asks the size
    // question because it answers a different one; do not swap them.
    //
    // ⚠ Both queries must be taken before getLastWindowSize() runs below. That call RECORDS the
    // size it hands back, so from then on a size query answers true to its own question ever
    // after. Moving either query down beside the branch that uses it makes it describe this
    // call instead of a previous one, which inverts its answer.
    const bool hasStoredPosition = GuiSettings::globalInstance()->widgetHasPersistentPosition(mdiSubWindow);
    const bool hasStoredSize = GuiSettings::globalInstance()->widgetHasPersistentGeometry(mdiSubWindow);

    QPoint pos = GuiSettings::globalInstance()->getLastWindowPosition(mdiSubWindow, window->defaultSize());
    QSize size = GuiSettings::globalInstance()->getLastWindowSize(mdiSubWindow, window->defaultSize());

    const QString kindName = QString("%1%2").arg(window->geometryKindName(), MdiSubWindow::MdiSubWindowSuffix);

    // Size is resolved independently of position. A window opened alongside others of its kind is
    // offset from them and is still the size the kind was last left at.
    if(hasStoredSize == false) {
        QSize kindSize;
        if(GuiSettings::globalInstance()->tryGetLastWindowSize(kindName, kindSize)) {
            size = kindSize;
        }
    }

    if(hasStoredPosition == false) {
        if(existing.count() > 0) {
            // position down and to the right a bit from the last existing
            static const int NewWindowOffset = 20;
            pos = QPoint(existing.last()->pos().x() + NewWindowOffset, existing.last()->pos().y() + NewWindowOffset);
        }
        else {
            // ⚠ getLastWindowPosition() has no MDI branch for a name it has never seen: it
            // returns a default-constructed QPoint rather than computing a placement.
            QPoint kindPosition;
            if(GuiSettings::globalInstance()->tryGetLastWindowPosition(kindName, kindPosition)) {
                pos = kindPosition;
            }
            else {
                pos = topLeftForChildWindow(size);
            }
        }
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

QPoint MdiWindow::topLeftForChildWindow(const QSize& windowSize)
{
    if(mdiArea() == nullptr) {
        return QPoint();
    }
    const QSize areaSize = mdiArea()->size();
    return QPoint(std::max((areaSize.width() / 2) - (windowSize.width() / 2), 0),
                  std::max((areaSize.height() / 2) - (windowSize.height() / 2), 0));
}

void MdiWindow::closeSubWindows(int type)
{
    for(MdiSubWindow* window : findMdiSubWindows(type)) {
        window->close();  // WA_DeleteOnClose handles deletion
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


#include "Kanoop/gui/moc_mdiwindow.cpp"
