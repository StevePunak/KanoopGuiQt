#ifndef GROUPBOX_H
#define GROUPBOX_H

#include <QGroupBox>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QGroupBox subclass with logging support.
 *
 * GroupBox adds LoggingBaseClass integration to QGroupBox, giving group boxes
 * access to the standard Kanoop logging infrastructure.
 */
class LIBKANOOPGUI_EXPORT GroupBox : public QGroupBox,
                                     public LoggingBaseClass
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QWidget parent
     */
    explicit GroupBox(QWidget *parent = nullptr);

    /**
     * @brief Construct with a title and optional parent.
     * @param title Group box title text
     * @param parent Optional QWidget parent
     */
    explicit GroupBox(const QString& title, QWidget *parent = nullptr);

signals:

};

#endif // GROUPBOX_H
