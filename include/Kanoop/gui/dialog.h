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

/**
 * @brief QDialog subclass providing logging, validation, persistence, and button-box management.
 *
 * Dialog wires common input widget signals to validate(), manages OK/Apply/Cancel
 * button enabling, optionally persists window position and size, and provides a
 * built-in QStatusBar. Subclasses must implement validate().
 */
class LIBKANOOPGUI_EXPORT Dialog : public QDialog,
               public LoggingBaseClass
{
    Q_OBJECT
public:
    /** @brief Construct with an optional parent. */
    explicit Dialog(QWidget* parent = nullptr);

    /**
     * @brief Construct with a logging category and optional parent.
     * @param loggingCategory Category name used for log output
     * @param parent Optional QWidget parent
     */
    explicit Dialog(const QString& loggingCategory, QWidget* parent = nullptr);

    /** @brief Destructor — closes any open log consumer. */
    virtual ~Dialog();

    /**
     * @brief Return whether the dialog persists its position between sessions.
     * @return true if position persistence is enabled
     */
    bool persistPosition() const { return _persistPosition; }

    /**
     * @brief Enable or disable position persistence.
     * @param value true to persist position
     */
    void setPersistPosition(bool value) { _persistPosition = value; }

    /**
     * @brief Return whether the dialog persists its size between sessions.
     * @return true if size persistence is enabled
     */
    bool persistSize() const { return _persistSize; }

    /**
     * @brief Enable or disable size persistence.
     * @param value true to persist size
     */
    void setPersistSize(bool value) { _persistSize = value; }

    /**
     * @brief Return whether to restore to the parent screen on show.
     * @return true if restore-to-parent-screen is enabled
     */
    bool restoreToParentScreen() const { return _restoreToParentScreen; }

    /**
     * @brief Enable or disable restore-to-parent-screen behaviour.
     * @param value true to restore to the parent screen
     */
    void setRestoreToParentScreen(bool value) { _restoreToParentScreen = value; }

protected:
    /** @brief Perform layout and button-box wiring; call from the subclass constructor. */
    void performLayout();

    /**
     * @brief Return whether the current form state is valid.
     * @return true if validate() last set the state to valid
     */
    bool isValid() const { return _valid; }

    /**
     * @brief Set the form validity state and update button enabling accordingly.
     * @param value true if the form is valid
     */
    void setValid(bool value);

    /**
     * @brief Return whether the form has unsaved changes.
     * @return true if the form is dirty
     */
    bool isDirty() const { return _dirty; }

    /**
     * @brief Set the dirty flag.
     * @param value true if the form has unsaved changes
     */
    void setDirty(bool value);

    /**
     * @brief Return whether the form failed to load.
     * @return true if form load failed
     */
    bool formLoadFailed() const { return _formLoadFailed; }

    /**
     * @brief Set the form load failure flag.
     * @param value true if form load failed
     */
    void setFormLoadFailed(bool value) { _formLoadFailed = value; }

    /**
     * @brief Return whether the form has finished loading.
     * @return true if form load is complete
     */
    bool formLoadComplete() const { return _formLoadComplete; }

    /**
     * @brief Enable or disable the Apply button.
     * @param value true to enable
     */
    void setApplyEnabled(bool value);

    /**
     * @brief Enable or disable the Cancel button.
     * @param value true to enable
     */
    void setCancelEnabled(bool value);

    /**
     * @brief Enable or disable the OK button.
     * @param value true to enable
     */
    void setOkEnabled(bool value);

    /**
     * @brief Show or hide the status bar.
     * @param value true to show
     */
    void setStatusBarVisible(bool value);

    /**
     * @brief Enable or disable the log consumer hook for this dialog.
     * @param enabled true to enable
     */
    void setLogHookEnabled(bool enabled);

    /** @brief Connect all child input widget signals to the validation slots. */
    void connectValidationSignals();

    /**
     * @brief Enable or disable all input child widgets of a given widget.
     * @param widget Parent widget whose children to enable/disable
     * @param enabled true to enable
     */
    void setChildInputWidgetsEnabled(QWidget* widget, bool enabled);

    /**
     * @brief Return the default text color from the current palette.
     * @return Default text QColor
     */
    QColor defaultTextColor() const { return palette().color(QPalette::Text); }

    /**
     * @brief Return the default dialog size.
     * @return Default QSize
     */
    QSize defaultSize() const { return _defaultSize; }

    /**
     * @brief Set the default dialog size.
     * @param value Default QSize
     */
    void setDefaultSize(const QSize& value) { _defaultSize = value; }

    /**
     * @brief Set the default dialog size by width and height.
     * @param width Default width in pixels
     * @param height Default height in pixels
     */
    void setDefaultSize(int width, int height) { _defaultSize = QSize(width, height); }

    /**
     * @brief Return the dialog button box.
     * @return Pointer to the QDialogButtonBox
     */
    QDialogButtonBox* buttonBox() const { return _buttonBox; }

    /**
     * @brief Return the dialog status bar.
     * @return Pointer to the QStatusBar
     */
    QStatusBar* statusBar() const { return _statusBar; }

    /** @brief Implement to validate the form and call setValid()/setDirty(). */
    virtual void validate() = 0;

    /**
     * @brief Called when a log entry is delivered (no-op by default).
     * @param entry The log entry
     */
    virtual void loggedItem(const Log::LogEntry& entry) { Q_UNUSED(entry) }

    /** @brief Return true if a != b (used with COMPARE macro). */
    bool compare(const QString& a, const QString& b) { return a != b; }
    /** @brief Return true if a != b (used with COMPARE macro). */
    bool compare(const QUuid& a, const QUuid& b) { return a != b; }
    /** @brief Return true if a != b (used with COMPARE macro). */
    bool compare(const QList<QUuid>& a, const QList<QUuid>& b) { return a != b; }
    /** @brief Return true if a != b (used with COMPARE macro). */
    bool compare(const int a, int b) { return a != b; }
    /** @brief Return true if a != b (used with COMPARE macro). */
    bool compare(const TimeSpan& a, const TimeSpan& b) { return a != b; }

    /** @brief Called when the OK button is clicked (no-op by default). */
    virtual void okClicked() {}
    /** @brief Called when the Apply button is clicked (no-op by default). */
    virtual void applyClicked() {}
    /** @brief Called when the Cancel button is clicked (no-op by default). */
    virtual void cancelClicked() {}

protected slots:
    /** @brief Update button enabling based on current validity and dirty state. */
    virtual void enableAppropriateButtons();

private:
    /** @brief Shared constructor initialization. */
    void commonInit();

protected:
    /** @brief Persist position on move. */
    virtual void moveEvent(QMoveEvent *event) override;
    /** @brief Persist size on resize. */
    virtual void resizeEvent(QResizeEvent *event) override;
    /** @brief Restore geometry and complete form load on first show. */
    virtual void showEvent(QShowEvent *event) override;

private:
    /** @brief Connect QLineEdit signals. */
    void connectLineEditSignals();
    /** @brief Connect QTextEdit signals. */
    void connectTextEditSignals();
    /** @brief Connect QComboBox signals. */
    void connectComboBoxSignals();
    /** @brief Connect QRadioButton signals. */
    void connectRadioButtonSignals();
    /** @brief Connect QCheckBox signals. */
    void connectCheckBoxSignals();
    /** @brief Connect QSpinBox signals. */
    void connectSpinBoxSignals();
    /** @brief Connect button-box button signals. */
    void connectButtonBoxSignals();
    /** @brief Create and configure the button box. */
    void setButtonBoxButtons();

    /** @brief Close and destroy the log consumer. */
    void closeLogConsumer();

    template <typename T>
    void appendToList(QList<QWidget*>& list, const QList<T>& typedList) const
    {
        for(T widget : typedList) {
            QWidget* w = qobject_cast<QWidget*>(widget);
            if(w != nullptr) {
                list.append(w);
            }
        }
    }

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
    /** @brief Emitted after an item is added via this dialog. */
    void itemAdded(const EntityMetadata& metadata);
    /** @brief Emitted after an item is deleted via this dialog. */
    void itemDeleted(const EntityMetadata& metadata);
    /** @brief Emitted after an item is updated via this dialog. */
    void itemUpdated(const EntityMetadata& metadata);

public slots:
    /** @brief Called when application preferences change; override to react. */
    virtual void onPreferencesChanged();

protected slots:
    /** @brief Trigger validation when a string input changes. */
    virtual void stringChanged(const QString&);
    /** @brief Trigger validation when an integer input changes. */
    virtual void intChanged(int);
    /** @brief Trigger validation when a boolean input changes. */
    virtual void boolChanged(bool);
    /** @brief Trigger validation when a parameterless signal fires. */
    virtual void voidChanged();

private slots:
    /** @brief Forward logged entries to loggedItem(). */
    void onLoggedItem(const Log::LogEntry& entry);
    /** @brief Persist splitter state when moved. */
    void onSplitterMoved();
    /** @brief Handle OK button click. */
    void onOkClicked();
    /** @brief Handle Apply button click. */
    void onApplyClicked();
    /** @brief Handle Cancel button click. */
    void onCancelClicked();
};

#endif // DIALOG_H
