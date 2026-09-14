#ifndef QOBJECTGRAPHICSITEM_H
#define QOBJECTGRAPHICSITEM_H

#include <Kanoop/gui/libkanoopgui.h>
#include <QObject>

/**
 * @brief Minimal QObject subclass serving as a QObject base for graphics item mixins.
 *
 * A plain QObject base for mixing signal/slot support into a QGraphicsItem subclass.
 */
class LIBKANOOPGUI_EXPORT QObjectGraphicsItem : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent.
     * @param parent Optional QObject parent
     */
    QObjectGraphicsItem(QObject* parent = nullptr);
};

#endif // QOBJECTGRAPHICSITEM_H
