#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QSlider subclass with an optional read-only mode.
 *
 * Slider exposes a readOnly property.  When set, mouse, keyboard and wheel input no
 * longer change the value, and the widget stays enabled.
 */
class LIBKANOOPGUI_EXPORT Slider : public QSlider
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit Slider(QWidget *parent = nullptr);

    /**
     * @brief Construct with a specific orientation.
     * @param orientation Qt::Horizontal or Qt::Vertical
     * @param parent Optional QWidget parent
     */
    explicit Slider(Qt::Orientation orientation, QWidget *parent = nullptr);

    /**
     * @brief Return whether the slider is read-only.
     * @return true if mouse, keyboard and wheel input are ignored
     */
    bool isReadOnly() const { return _readOnly; }

    /**
     * @brief Enable or disable the read-only mode.
     * @param value true to suppress mouse, keyboard and wheel input
     */
    void setReadOnly(bool value) { _readOnly = value; }

signals:

protected:
    /** @brief Suppress mouse presses when in read-only mode. */
    virtual void mousePressEvent(QMouseEvent* event) override;
    /** @brief Suppress key presses when in read-only mode. */
    virtual void keyPressEvent(QKeyEvent* event) override;
    /** @brief Suppress key releases when in read-only mode. */
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    /** @brief Suppress wheel input when in read-only mode. */
    virtual void wheelEvent(QWheelEvent* event) override;

private:
    bool _readOnly = false;
};

#endif // SLIDER_H
