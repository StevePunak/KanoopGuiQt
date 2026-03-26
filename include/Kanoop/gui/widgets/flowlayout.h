#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QGridLayout>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT FlowLayout : public QGridLayout
{
    Q_OBJECT
public:
    explicit FlowLayout(QWidget *parent = nullptr);

    void addWidget(QWidget* widget);

    int maxColumns() const { return _maxColumns; }
    void setMaxColumns(int value) { _maxColumns = value; }

    void compact();
    void clear();

private:
    int _maxColumns;

signals:

};

#endif // FLOWLAYOUT_H
