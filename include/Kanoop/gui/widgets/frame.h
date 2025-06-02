#ifndef FRAME_H
#define FRAME_H

#include <QFrame>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT Frame : public QFrame,
                                  public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit Frame(QWidget *parent = nullptr);

signals:

};

#endif // FRAME_H
