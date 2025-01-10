#ifndef QUATERNION_H
#define QUATERNION_H
#include <QQuaternion>

class Quaternion : public QQuaternion
{
public:
    constexpr Quaternion() noexcept :
        QQuaternion() {}
    explicit Quaternion(Qt::Initialization initialization) noexcept :
        QQuaternion(initialization) {}
    constexpr Quaternion(float scalar, float xpos, float ypos, float zpos) noexcept :
        QQuaternion(scalar, xpos, ypos, zpos) {}
#ifndef QT_NO_VECTOR3D
    constexpr Quaternion(float scalar, const QVector3D &vector) noexcept :
        QQuaternion(scalar, vector) {}
#endif
#ifndef QT_NO_VECTOR4D
    constexpr explicit Quaternion(const QVector4D &vector) noexcept :
        QQuaternion(vector) {}
#endif

    QString toString() const { return QString("%1, %2, %3, %4").arg(x()).arg(y()).arg(z()).arg(scalar()); }
    static Quaternion fromString(const QString& value);
};

#endif // QUATERNION_H
