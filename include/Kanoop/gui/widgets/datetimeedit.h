#ifndef DATETIMEEDIT_H
#define DATETIMEEDIT_H

#include <QDateTimeEdit>

class DateTimeEdit : public QDateTimeEdit
{
    Q_OBJECT
public:
    explicit DateTimeEdit(QWidget *parent = nullptr);

signals:

};

#endif // DATETIMEEDIT_H
