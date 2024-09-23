/******************************************************************************************
**
** mainwindowbase.h
**
** Copyright (C) Tesseract Engineering, Inc - All Rights Reserved
**
** This source code is protected under international copyright law.  All rights
** reserved and protected by the copyright holders.
**
** This file is confidential and only available to authorized individuals with the
** permission of the copyright holders.  If you encounter this file and do not have
** permission, please contact the copyright holders and delete this file.
**
** Author:  Stephen Punak
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#ifndef MAINWINDOWBASE_H
#define MAINWINDOWBASE_H

#include <QMainWindow>

#include <Kanoop/utility/loggingbaseclass.h>

class QMdiArea;
class MainWindowBase : public QMainWindow,
                       public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit MainWindowBase(const QString& loggingCategory, QWidget *parent = nullptr);

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
    bool _formLoadComplete;
    bool _formLoadFailed;

signals:

public slots:
    virtual void onPreferencesChanged();

private slots:
    void onSpliltterMoved();

};

#endif // MAINWINDOWBASE_H
