#ifndef FRAME_H
#define FRAME_H

#include <QFrame>

#include <Kanoop/utility/loggingbaseclass.h>

class Frame : public QFrame,
              public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit Frame(QWidget *parent = nullptr);

signals:

};

#endif // FRAME_H
