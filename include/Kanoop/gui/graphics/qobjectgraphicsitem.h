#ifndef QOBJECTGRAPHICSITEM_H
#define QOBJECTGRAPHICSITEM_H

#include <QObject>

/**
 * @brief Minimal QObject subclass serving as a QObject base for graphics item mixins.
 *
 * QObjectGraphicsItem provides a QObject foundation that can be used alongside
 * QGraphicsItem subclasses to add signal/slot support without using QGraphicsObject
 * (which carries additional overhead).
 */
class QObjectGraphicsItem : public QObject
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
