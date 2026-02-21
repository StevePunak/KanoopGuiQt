#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QGraphicsScene subclass with logging support and typed-item search helpers.
 *
 * GraphicsScene adds findChildItems<T>() and findFirstChildItem<T>() template
 * helpers that search all items in the scene and return only those that dynamic_cast
 * to the requested pointer type.
 */
class LIBKANOOPGUI_EXPORT GraphicsScene : public QGraphicsScene,
                                          public LoggingBaseClass
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent object.
     * @param parent Optional QObject parent
     */
    explicit GraphicsScene(QObject *parent = nullptr);

    /**
     * @brief Return all scene items that dynamic_cast to type T.
     * @tparam T Pointer type to search for (must derive from QGraphicsItem)
     * @return List of matching items cast to T
     */
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

    /**
     * @brief Return the first scene item that dynamic_cast to type T.
     * @tparam T Pointer type to search for (must derive from QGraphicsItem)
     * @return First matching item cast to T, or nullptr if none found
     */
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
