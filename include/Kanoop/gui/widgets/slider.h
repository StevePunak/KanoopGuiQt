#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>

class Slider : public QSlider
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
