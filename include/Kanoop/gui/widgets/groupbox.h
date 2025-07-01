#ifndef GROUPBOX_H
#define GROUPBOX_H

#include <QGroupBox>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT GroupBox : public QGroupBox,
                                     public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit GroupBox(QWidget *parent = nullptr);
    explicit GroupBox(const QString& title, QWidget *parent = nullptr);

signals:

};

#endif // GROUPBOX_H
