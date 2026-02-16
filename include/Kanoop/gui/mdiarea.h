#ifndef MDIAREA_H
#define MDIAREA_H
#include <QMdiArea>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT MdiArea : public QMdiArea
{
    Q_OBJECT
public:
    explicit MdiArea(QWidget* parent = nullptr);

signals:
    void subWindowPaging(QMdiSubWindow* subWindow);
    void pagingComplete();

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QMdiSubWindow *getNextSubWindowInCycle(bool forward = true);

    int _pagingIndex = -1;
};

#endif // MDIAREA_H
