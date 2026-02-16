#include "mdiarea.h"

#include <QKeyEvent>
#include <QWindow>
#include <Kanoop/log.h>

MdiArea::MdiArea(QWidget* parent) :
    QMdiArea(parent)
{
}

bool MdiArea::eventFilter(QObject* watched, QEvent* event)
{
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->modifiers() & Qt::ControlModifier) {
            QWindow* baseWindow = qobject_cast<QWindow*>(watched);
            if(baseWindow != nullptr) {
                QMdiSubWindow* nextWindow = nullptr;
                if(keyEvent->key() == Qt::Key_Tab) {
                    nextWindow = getNextSubWindowInCycle(true);
                }
                else if(keyEvent->key() == Qt::Key_Backtab) {
                    nextWindow = getNextSubWindowInCycle(false);
                }
                if(nextWindow != nullptr) {
                    emit subWindowPaging(nextWindow);
                }
            }
        }
    }
    else if(event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if(_pagingIndex >= 0 && keyEvent->key() == Qt::Key_Control) {
            _pagingIndex = -1;
            emit pagingComplete();
        }
    }
    return QMdiArea::eventFilter(watched, event);
}

QMdiSubWindow* MdiArea::getNextSubWindowInCycle(bool forward)
{
    // Get current list in the cycling order
    QList<QMdiSubWindow*> windows = subWindowList(activationOrder());

    if(windows.count() <= 1) {
        return nullptr;
    }

    QMdiSubWindow* current = activeSubWindow();
    if(!current) {
        return forward ? windows.first() : windows.last();
    }

    if(_pagingIndex < 0) {
        _pagingIndex = windows.indexOf(activeSubWindow());
    }

    if(_pagingIndex >= 0 && _pagingIndex < windows.count()) {
        if(forward) {
            _pagingIndex = (_pagingIndex + 1) % windows.size();
        }
        else {
            _pagingIndex = (_pagingIndex - 1 + windows.size()) % windows.size();
        }
        return windows.at(_pagingIndex);
    }

    return nullptr;  // should not happen
}
