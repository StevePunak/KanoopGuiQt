/******************************************************************************************
**
** mainwindowbase.h
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#ifndef MAINWINDOWBASE_H
#define MAINWINDOWBASE_H

#include <QMainWindow>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class QMdiArea;
class LIBKANOOPGUI_EXPORT MainWindowBase : public QMainWindow,
                                           public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit MainWindowBase(const QString& loggingCategory, QWidget *parent = nullptr);

    bool persistPosition() const { return _persistPosition; }
    void setPersistPosition(bool value) { _persistPosition = value; }

    bool persistSize() const { return _persistSize; }
    void setPersistSize(bool value) { _persistSize = value; }

    int type() const { return _type; }
    void setType(int value) { _type = value; }

    QSize defaultSize() const { return _defaultSize; }
    void setDefaultSize(const QSize& value) { _defaultSize = value; }
    void setDefaultSize(int width, int height) { _defaultSize = QSize(width, height); }

protected:
    void initializeBase();

    bool formLoadComplete() const { return _formLoadComplete; }
    void setFormLoadComplete(bool value) { _formLoadComplete = value; }

    bool formLoadFailed() const { return _formLoadFailed; }
    void setFormLoadFailed(bool value) { _formLoadFailed = value; }

    QMdiArea* parentMdiArea();

    // QWidget interface
    virtual void moveEvent(QMoveEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

private:
    int _type = 0;
    bool _formLoadComplete = false;
    bool _formLoadFailed = false;
    bool _persistPosition = true;
    bool _persistSize = true;
    QSize _defaultSize;

signals:

public slots:
    virtual void onPreferencesChanged();

private slots:
    void onSpliltterMoved();

};

#endif // MAINWINDOWBASE_H
