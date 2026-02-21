#ifndef COMPLEXWIDGET_H
#define COMPLEXWIDGET_H

#include <QWidget>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QWidget subclass that adds logging support and auto-wired input validation.
 *
 * ComplexWidget automatically connects common input widget signals (QLineEdit,
 * QComboBox, QRadioButton, QCheckBox, QSpinBox) to the validate() slot so that
 * subclasses only need to override validate() to implement form validation.
 * Call initializeBase() from the subclass constructor after building the UI.
 */
class LIBKANOOPGUI_EXPORT ComplexWidget : public QWidget,
                      public LoggingBaseClass
{
    Q_OBJECT
public:
    /** @brief Construct with an optional parent. */
    explicit ComplexWidget(QWidget *parent = nullptr);

    /**
     * @brief Construct with a logging category and optional parent.
     * @param loggingCategory Category name used for log output
     * @param parent Optional QWidget parent
     */
    explicit ComplexWidget(const QString& loggingCategory, QWidget *parent = nullptr);

private:
    /** @brief Shared constructor initialization. */
    void commonInit();

protected:
    /** @brief Perform base-class initialization; call after building the UI. */
    void initializeBase();
    /** @brief Connect all child input widget signals to validate(). */
    void connectValidationSignals();

protected slots:
    /** @brief Override to implement form validation logic. */
    virtual void validate() {}

private:
    /** @brief Restore widget geometry from persisted settings. */
    void restorePersistedSettings();
    /** @brief Connect QLineEdit signals to validate(). */
    void connectLineEditSignals();
    /** @brief Connect QComboBox signals to validate(). */
    void connectComboBoxSignals();
    /** @brief Connect QRadioButton signals to validate(). */
    void connectRadioButtonSignals();
    /** @brief Connect QCheckBox signals to validate(). */
    void connectCheckBoxSignals();
    /** @brief Connect QSpinBox signals to validate(). */
    void connectSpinBoxSignals();

public slots:
    /** @brief Called when application preferences change; override to react. */
    virtual void onPreferencesChanged();

private slots:
    /** @brief React to the widget's objectName changing. */
    void onObjectNameChanged();
    /** @brief React to a QSplitter being moved. */
    virtual void onSplitterMoved();
    /** @brief Delegate string-change events to validate(). */
    virtual void stringChanged(const QString&) { validate(); }
    /** @brief Delegate int-change events to validate(). */
    virtual void intChanged(int) { validate(); }
    /** @brief Delegate bool-change events to validate(). */
    virtual void boolChanged(bool) { validate(); }
    /** @brief Delegate parameterless-change events to validate(). */
    virtual void voidChanged() { validate(); }
signals:

};

#endif // COMPLEXWIDGET_H
