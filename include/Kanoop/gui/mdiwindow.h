#ifndef MDIWINDOW_H
#define MDIWINDOW_H
#include <Kanoop/gui/mainwindowbase.h>
#include <Kanoop/gui/libkanoopgui.h>

class QMdiSubWindow;
class MdiSubWindow;
class LIBKANOOPGUI_EXPORT MdiWindow : public MainWindowBase
{
    Q_OBJECT
public:
    MdiWindow(const QString& logPrefix, QWidget* parent = nullptr);

protected:
    MainWindowBase* activeSubWindow();
    virtual void openSubWindow(MainWindowBase* window, int type);
    virtual void closeSubWindows(int type);

    MdiSubWindow* findFirstMdiSubWindow(int type) const;
    QList<MdiSubWindow*> findMdiSubWindows(int type) const;

    QMdiArea* mdiArea();

    virtual void subWindowClosing(MdiSubWindow* window) { Q_UNUSED(window) }

private:
    QMdiArea* _mdiArea = nullptr;

private slots:
    void onSubWindowClosing();
};

#endif // MDIWINDOW_H
