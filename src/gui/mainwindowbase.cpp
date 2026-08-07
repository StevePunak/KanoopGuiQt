/******************************************************************************************
**
** mainwindowbase.cpp
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#include "guisettings.h"
#include "mainwindowbase.h"

#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMdiArea>
#include <QMenu>
#include <QMoveEvent>
#include <QResizeEvent>
#include <QScreen>
#include <QSplitter>
#include <QStatusBar>
#include <QTimer>
#include "mdisubwindow.h"

#include <Kanoop/geometry/point.h>
#include <Kanoop/geometry/size.h>

#include <algorithm>

MainWindowBase::MainWindowBase(const QString &loggingCategory, QWidget *parent) :
    QMainWindow{parent},
    LoggingBaseClass(loggingCategory),
    _formLoadComplete(false), _formLoadFailed(false)
{
    MainWindowBase::setObjectName(MainWindowBase::metaObject()->className());
    setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindowBase::showStatusBarMessage(const QString& text, const QColor& textColor, const TimeSpan& timeout)
{
    statusBar()->showStatusMessage(text, textColor, timeout);
}

void MainWindowBase::showStatusBarMessage(const QString& text, const TimeSpan& timeout)
{
    statusBar()->showStatusMessage(text, palette().color(QPalette::Text), timeout);
}

void MainWindowBase::showStatusBarAnimatedProgressMessage(const QString& text, const QColor& textColor)
{
    statusBar()->showAnimatedProgressMessage(text, textColor);
}

void MainWindowBase::stopStatusBarAnimation()
{
    statusBar()->stopAnimation();
}

void MainWindowBase::initializeBase()
{
    onPreferencesChanged();

    if(GuiSettings::globalInstance() != nullptr) {
        for(QSplitter* splitter : findChildren<QSplitter*>()) {
            GuiSettings::globalInstance()->restoreLastSplitterState(splitter);
            connect(splitter, &QSplitter::splitterMoved, this, &MainWindowBase::onSpliltterMoved);
        }
    }
}

QMdiArea *MainWindowBase::parentMdiArea()
{
    QMdiArea* result = nullptr;
    QObject* parentWidget = parent();
    while(parentWidget != nullptr && result == nullptr) {
        result = dynamic_cast<QMdiArea*>(parentWidget);
        parentWidget = parentWidget->parent();
    }
    return result;
}

StatusBar* MainWindowBase::statusBar()
{
    if(_statusBar == nullptr) {
        _statusBar = new StatusBar(this);
        setStatusBar(_statusBar);
    }
    return _statusBar;
}

void MainWindowBase::moveEvent(QMoveEvent *event)
{
    // logText(LVL_DEBUG, QString("%1 - move to %2").arg(objectName()).arg(Point(event->pos()).toString()));
    if(_formLoadComplete && _persistPosition) {
        GuiSettings::globalInstance()->setLastWindowPosition(this, event->pos());
    }
    QMainWindow::moveEvent(event);
}

void MainWindowBase::resizeEvent(QResizeEvent *event)
{
    // logText(LVL_DEBUG, QString("%1 - resize to %2").arg(objectName()).arg(Size(event->size()).toString()));
    if(_formLoadComplete && _persistSize) {
        GuiSettings::globalInstance()->setLastWindowSize(this, event->size());
    }
    QMainWindow::resizeEvent(event);
}

void MainWindowBase::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    if(_formLoadComplete == false) {
        if(_persistPosition || _persistSize) {
            QWidget* parent = parentWidget();
            bool isMdiSubWindow = qobject_cast<MdiSubWindow*>(parent) != nullptr;
            QRect geometryRect = parent != nullptr ? parent->geometry() : geometry();

            if(_persistPosition) {
                geometryRect.setTopLeft(GuiSettings::globalInstance()->getLastWindowPosition(this, _defaultSize));
            }
            if(_persistSize) {
                geometryRect.setSize(GuiSettings::globalInstance()->getLastWindowSize(this, _defaultSize));
            }

            // MDI subwindows take care of themselves
            if(isMdiSubWindow == false) {
                // Ensure the restore point is on a connected screen. A stale position can
                // reference a monitor which has since been removed.
                QScreen* screen = QGuiApplication::screenAt(geometryRect.topLeft());
                if(screen == nullptr) {
                    logText(LVL_DEBUG, QString("The restore point is off the screen - centering on primary screen"));
                    screen = QGuiApplication::primaryScreen();
                    QRect screenRect = screen->availableGeometry();
                    QPoint centered = screenRect.center();
                    centered.rx() -= (geometryRect.width() / 2);
                    centered.ry() -= (geometryRect.height() / 2);
                    geometryRect.moveTopLeft(centered);
                }

                // The restored size itself was never validated. It can exceed the current screen
                // because it was saved on a larger monitor, or because the default size is simply
                // bigger than a laptop panel. Nothing downstream bounds it and the overflow sits
                // off-screen where it cannot be reached, so bound it to the work area here.
                if(screen != nullptr) {
                    geometryRect = boundToScreen(geometryRect, screen);
                }

                if(parent != nullptr) {
                    parent->resize(geometryRect.size());
                    parent->move(geometryRect.topLeft());
                }
                else {
                    resize(geometryRect.size());
                    move(geometryRect.topLeft());
                }
            }
        }
        _formLoadComplete = true;
        if(_formLoadFailed) {
            QTimer::singleShot(0, parentWidget(), &QWidget::close);
        }
    }
    QMainWindow::showEvent(event);
}

QRect MainWindowBase::boundToScreen(const QRect& geometryRect, const QScreen* screen)
{
    QRect result = geometryRect;
    QRect available = screen->availableGeometry();

    QSize bounded = result.size().boundedTo(available.size());
    if(bounded != result.size()) {
        logText(LVL_INFO, QString("Restored size %1 exceeds the available screen area %2 - bounding to %3")
                              .arg(Size(result.size()).toString())
                              .arg(Size(available.size()).toString())
                              .arg(Size(bounded).toString()));
        result.setSize(bounded);
    }

    // Guard the other direction for windows that asked for a floor. A window shrunk to a handful
    // of pixels persists that size and reopens unusable. The work area still wins, so this never
    // forces a window larger than the screen.
    if(_minimumRestoreSize.isValid()) {
        QSize raised = result.size().expandedTo(_minimumRestoreSize).boundedTo(available.size());
        if(raised != result.size()) {
            logText(LVL_INFO, QString("Restored size %1 is below the minimum restore size %2 - raising to %3")
                                  .arg(Size(result.size()).toString())
                                  .arg(Size(_minimumRestoreSize).toString())
                                  .arg(Size(raised).toString()));
            result.setSize(raised);
        }
    }

    // Now that the size fits, slide the window back inside the work area if the restore point
    // put part of it beyond the right or bottom edge.
    QPoint topLeft = result.topLeft();
    topLeft.setX(std::min(topLeft.x(), available.right() - result.width() + 1));
    topLeft.setY(std::min(topLeft.y(), available.bottom() - result.height() + 1));
    topLeft.setX(std::max(topLeft.x(), available.left()));
    topLeft.setY(std::max(topLeft.y(), available.top()));
    if(topLeft != result.topLeft()) {
        logText(LVL_INFO, QString("Restored position %1 puts the window outside the work area - moving to %2")
                              .arg(Point(result.topLeft()).toString())
                              .arg(Point(topLeft).toString()));
        result.moveTopLeft(topLeft);
    }

    return result;
}

void MainWindowBase::onPreferencesChanged()
{
    if(GuiSettings::globalInstance() == nullptr) {
        return;
    }

    QFont newFont(font());
    int pointSize = GuiSettings::globalInstance()->fontSize();
    newFont.setPointSize(pointSize);
    setFont(newFont);

    // Force all child widgets to update their fonts
    for (QWidget* widget : findChildren<QWidget*>()) {
        if (widget != this) {
            widget->setFont(newFont);
        }
    }
}

void MainWindowBase::onSpliltterMoved()
{
    GuiSettings::globalInstance()->saveLastSplitterState(static_cast<QSplitter*>(sender()));
}

#include "Kanoop/gui/moc_mainwindowbase.cpp"
