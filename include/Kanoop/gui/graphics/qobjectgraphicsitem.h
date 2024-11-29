#ifndef QOBJECTGRAPHICSITEM_H
#define QOBJECTGRAPHICSITEM_H

#include <QObject>

class QObjectGraphicsItem : public QObject
{
    Q_OBJECT
public:
    QObjectGraphicsItem(QObject* parent = nullptr);
};

#endif // QOBJECTGRAPHICSITEM_H
