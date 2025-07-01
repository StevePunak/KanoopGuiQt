#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT ComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ComboBox(QWidget *parent = nullptr);

    void setBold(bool bold);
    void setRowBold(int row, bool bold);

private:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;

signals:
    void accept();
    void lostFocus();
};

#endif // COMBOBOX_H
