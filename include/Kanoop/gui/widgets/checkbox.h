#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <QCheckBox>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT CheckBox : public QCheckBox
{
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)             // clazy:exclude=qproperty-without-notify
    Q_OBJECT
public:
    explicit CheckBox(QWidget *parent = nullptr);
    explicit CheckBox(const QString &text, QWidget *parent = nullptr);

    bool isReadOnly() const { return _readOnly; }

public slots:
    void setReadOnly(bool value) { _readOnly = value; }

private:
    virtual void mousePressEvent(QMouseEvent* event) override;

private:
    bool _readOnly = false;
};

#endif // CHECKBOX_H
