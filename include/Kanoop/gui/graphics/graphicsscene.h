#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT GraphicsScene : public QGraphicsScene,
                                          public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = nullptr);

    template <typename T>
    QList<T> findChildItems() const
    {
        QList<T> result;
        for(QGraphicsItem* item : items()) {
            T candidate = dynamic_cast<T>(item);
            if(candidate != nullptr) {
                result.append(candidate);
            }
        }
        return result;
    }

    template <typename T>
    T findFirstChildItem() const
    {
        T result = nullptr;
        for(QGraphicsItem* item : items()) {
            T candidate = dynamic_cast<T>(item);
            if(candidate != nullptr) {
                result = candidate;
                break;
            }
        }
        return result;
    }

signals:

};

#endif // GRAPHICSSCENE_H
