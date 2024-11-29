#ifndef VECTOR3D_H
#define VECTOR3D_H
#include <QVector3D>

class Vector3D : public QVector3D
{
public:
    constexpr Vector3D() noexcept :
        QVector3D() {}
    explicit Vector3D(Qt::Initialization initialization) noexcept :
        QVector3D(initialization) {}
    constexpr Vector3D(float xpos, float ypos, float zpos) noexcept :
        QVector3D(xpos, ypos, zpos) {}

    constexpr explicit Vector3D(QPoint point) noexcept :
        QVector3D(point) {}
    constexpr explicit Vector3D(QPointF point) noexcept :
        QVector3D(point) {}
#ifndef QT_NO_VECTOR2D
    constexpr explicit Vector3D(QVector2D vector) noexcept :
        QVector3D(vector) {}
    constexpr Vector3D(QVector2D vector, float zpos) noexcept :
        QVector3D(vector, zpos) {}
#endif
#ifndef QT_NO_VECTOR4D
    constexpr explicit Vector3D(QVector4D vector) noexcept :
        QVector3D(vector) {}
#endif

    QString toString() const { return QString("%1, %2, %3").arg(x()).arg(y()).arg(z()); }
    static Vector3D fromString(const QString& value);

};

#endif // VECTOR3D_H
