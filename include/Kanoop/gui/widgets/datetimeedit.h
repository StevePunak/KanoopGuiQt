#ifndef DATETIMEEDIT_H
#define DATETIMEEDIT_H

#include <QDateTimeEdit>

/**
 * @brief QDateTimeEdit subclass reserved for future extension.
 *
 * DateTimeEdit currently adds no extra functionality beyond QDateTimeEdit
 * but provides a consistent project-local class for future enhancements.
 */
class DateTimeEdit : public QDateTimeEdit
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit DateTimeEdit(QWidget *parent = nullptr);

signals:

};

#endif // DATETIMEEDIT_H
