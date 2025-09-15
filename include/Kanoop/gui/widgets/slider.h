#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT Slider : public QSlider
{
    Q_OBJECT
public:
    explicit Slider(QWidget *parent = nullptr);
    explicit Slider(Qt::Orientation orientation, QWidget *parent = nullptr);

    bool isReadOnly() const { return _readOnly; }
    void setReadOnly(bool value) { _readOnly = value; }

signals:

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;

private:
    bool _readOnly = false;
};

#endif // SLIDER_H
