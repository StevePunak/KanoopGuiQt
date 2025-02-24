#ifndef MDISUBWINDOW_H
#define MDISUBWINDOW_H
#include <Kanoop/gui/libkanoopgui.h>
#include <QMdiSubWindow>

class LIBKANOOPGUI_EXPORT MdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit MdiSubWindow(QWidget* parent = nullptr);

    int type() const { return _type; }
    void setType(int value) { _type = value; }

private:
    virtual void moveEvent(QMoveEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void closeEvent(QCloseEvent* event) override;

    bool _formLoadComplete = false;
    int _type = 0;

signals:
    void closing();

public slots:
    virtual void onPreferencesChanged();
};

#endif // MDISUBWINDOW_H
