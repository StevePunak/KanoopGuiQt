/******************************************************************************************
**
** dialogbase.h
**
** Moved out of Tesseract and made open source
**
** Author:  Stephen Punak
** Created: Sun Oct 22 17:41:53 2023
**
******************************************************************************************/
#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>
#include <QDialogButtonBox>
#include <QStatusBar>
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
    virtual ~Dialog();

    bool persistPosition() const { return _persistPosition; }
    void setPersistPosition(bool value) { _persistPosition = value; }

    bool persistSize() const { return _persistSize; }
    void setPersistSize(bool value) { _persistSize = value; }

    bool restoreToParentScreen() const { return _restoreToParentScreen; }
    void setRestoreToParentScreen(bool value) { _restoreToParentScreen = value; }

protected:
    void performLayout();

    bool isValid() const { return _valid; }
    void setValid(bool value);

    bool isDirty() const { return _dirty; }
    void setDirty(bool value);

    bool formLoadFailed() const { return _formLoadFailed; }
    void setFormLoadFailed(bool value) { _formLoadFailed = value; }

    void setApplyEnabled(bool value);
    void setCancelEnabled(bool value);
    void setOkEnabled(bool value);
    void setStatusBarVisible(bool value);

    void setLogHookEnabled(bool enabled);

    void connectValidationSignals();

    QColor defaultTextColor() const { return palette().color(QPalette::Text); }

    QSize defaultSize() const { return _defaultSize; }
    void setDefaultSize(const QSize& value) { _defaultSize = value; }
    void setDefaultSize(int width, int height) { _defaultSize = QSize(width, height); }

    QDialogButtonBox* buttonBox() const { return _buttonBox; }
    QStatusBar* statusBar() const { return _statusBar; }

    virtual void validate() = 0;

    virtual void loggedItem(const Log::LogEntry& entry) { Q_UNUSED(entry) }

    bool compare(const QString& a, const QString& b) { return a != b; }
    bool compare(const QUuid& a, const QUuid& b) { return a != b; }
    bool compare(const QList<QUuid>& a, const QList<QUuid>& b) { return a != b; }
    bool compare(const int a, int b) { return a != b; }
    bool compare(const TimeSpan& a, const TimeSpan& b) { return a != b; }

    virtual void okClicked() {}
    virtual void applyClicked() {}
    virtual void cancelClicked() {}

protected slots:
    virtual void enableAppropriateButtons();

private:
    void commonInit();

protected:
    virtual void moveEvent(QMoveEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

private:
    void connectLineEditSignals();
    void connectComboBoxSignals();
    void connectRadioButtonSignals();
    void connectCheckBoxSignals();
    void connectSpinBoxSignals();
    void connectButtonBoxSignals();
    void setButtonBoxButtons();

    void closeLogConsumer();

    QDialogButtonBox* _buttonBox = nullptr;
    QStatusBar* _statusBar = nullptr;
    bool _formLoadComplete = false;
    bool _formLoadFailed = false;
    bool _valid = false;
    bool _dirty = false;

    bool _applyEnabled = true;
    bool _cancelEnabled = true;
    bool _okEnabled = true;

    QSize _defaultSize;

    bool _persistPosition = true;
    bool _persistSize = true;
    bool _restoreToParentScreen = true;

    LogConsumer* _logConsumer  = nullptr;

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
    void onLoggedItem(const Log::LogEntry& entry);
    void onOkClicked();
    void onApplyClicked();
    void onCancelClicked();
};

#endif // DIALOG_H
