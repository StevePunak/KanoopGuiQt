#ifndef SPINNERWIDGET_H
#define SPINNERWIDGET_H
#include <QTimer>
#include <QFrame>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief Animated spinner widget for indicating background activity.
 *
 * SpinnerWidget renders a rotating set of pre-built pixmaps driven by an
 * internal QTimer.  An optional numeric value can be painted in the center.
 * Call setSpinning(true) to start the animation and setSpinning(false) to stop.
 */
class LIBKANOOPGUI_EXPORT SpinnerWidget : public QFrame
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit SpinnerWidget(QWidget* parent = nullptr);

    /**
     * @brief Return the integer value painted in the center of the spinner.
     * @return Current value
     */
    int value() const { return _value; }

    /**
     * @brief Return whether the optional text overlay is visible.
     * @return true if text is shown
     */
    bool isTextVisible() const { return _textVisible; }

    /**
     * @brief Return whether the spinner animation is currently running.
     * @return true if the timer is active
     */
    bool isSpinning() const { return _timer.isActive(); }

public slots:
    /**
     * @brief Start or stop the spinner animation.
     * @param value true to start, false to stop
     */
    void setSpinning(bool value);

    /**
     * @brief Set the numeric value displayed in the center.
     * @param value Integer to paint
     */
    void setValue(int value) { _value = value; update(); }

    /**
     * @brief Show or hide the center text overlay.
     * @param value true to show, false to hide
     */
    void setTextVisible(bool value) { _textVisible = value; update(); }

private:
    void buildPixmaps();
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

    int _value;
    int _index;
    bool _textVisible;

    QList<QPixmap> _pixmaps;
    QTimer _timer;

    static const double DegreesPerTick;

private slots:
    void onSpinTimer();
};

#endif // SPINNERWIDGET_H
