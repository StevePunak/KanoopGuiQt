/******************************************************************************************
**
** dialogbase.h
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
** Created: Sun Oct 22 17:41:53 2023
**
******************************************************************************************/
#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>
#include <QDialogButtonBox>
#include <QUuid>
#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/entitymetadata.h>
#include <Kanoop/timespan.h>
#include <Kanoop/gui/libkanoopgui.h>

#define COMPARE(a, b)       compare(a, b)

class LIBKANOOPGUI_EXPORT Dialog : public QDialog,
               public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit Dialog(QWidget* parent = nullptr);
    explicit Dialog(const QString& loggingCategory, QWidget* parent = nullptr);

protected:
    void performLayout();

    bool isValid() const { return _valid; }
    void setValid(bool value);

    bool isDirty() const { return _dirty; }
    void setDirty(bool value);

    bool formLoadFailed() const { return _formLoadFailed; }
    void setFormLoadFailed(bool value) { _formLoadFailed = value; }

    void setApplyEnabled(bool value);

    void connectValidationSignals();

    virtual void validate() = 0;

    bool compare(const QString& a, const QString& b) { return a != b; }
    bool compare(const QUuid& a, const QUuid& b) { return a != b; }
    bool compare(const QList<QUuid>& a, const QList<QUuid>& b) { return a != b; }
    bool compare(const int a, int b) { return a != b; }
    bool compare(const TimeSpan& a, const TimeSpan& b) { return a != b; }

    virtual void enableAppropriateButtons();

    virtual void okClicked() {}
    virtual void applyClicked() {}
    virtual void cancelClicked() {}

private:
    void commonInit();

    virtual void moveEvent(QMoveEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

    void connectLineEditSignals();
    void connectComboBoxSignals();
    void connectButtonBoxSignals();

    QDialogButtonBox* _buttonBox;
    bool _formLoadComplete;
    bool _formLoadFailed;
    bool _valid;
    bool _dirty;

signals:
    void itemAdded(const EntityMetadata& metadata);
    void itemDeleted(const EntityMetadata& metadata);
    void itemUpdated(const EntityMetadata& metadata);

public slots:
    virtual void onPreferencesChanged();

protected slots:
    virtual void stringChanged(const QString&);
    virtual void intChanged(int);
    virtual void boolChanged(bool);
    virtual void voidChanged();

private slots:
    void onOkClicked();
    void onApplyClicked();
    void onCancelClicked();
};

#endif // DIALOG_H
