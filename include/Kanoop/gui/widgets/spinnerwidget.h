#ifndef SPINNERWIDGET_H
#define SPINNERWIDGET_H
#include <QTimer>
#include <QFrame>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT SpinnerWidget : public QFrame
{
    Q_OBJECT
public:
    explicit SpinnerWidget(QWidget* parent = nullptr);

    int value() const { return _value; }
    bool isTextVisible() const { return _textVisible; }
    bool isSpinning() const { return _timer.isActive(); }

public slots:
    void setSpinning(bool value);
    void setValue(int value) { _value = value; update(); }
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
