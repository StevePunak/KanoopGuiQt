#include "widgets/datetimeedit.h"

#include <QLineEdit>


DateTimeEdit::DateTimeEdit(QWidget *parent) :
    QDateTimeEdit{parent}
{
}

#include "Kanoop/gui/widgets/moc_datetimeedit.cpp"
