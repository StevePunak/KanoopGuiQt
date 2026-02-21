#ifndef QUATERNION_H
#define QUATERNION_H
#include <QQuaternion>

/**
 * @brief QQuaternion subclass adding string serialization helpers.
 *
 * Quaternion forwards all QQuaternion constructors and adds toString() for
 * human-readable output and fromString() for round-trip parsing.
 */
class Quaternion : public QQuaternion
{
public:
    /** @brief Default constructor — identity quaternion. */
    constexpr Quaternion() noexcept :
        QQuaternion() {}
    /**
     * @brief Construct with explicit initialization.
     * @param initialization Qt initialization flag
     */
    explicit Quaternion(Qt::Initialization initialization) noexcept :
        QQuaternion(initialization) {}
    /**
     * @brief Construct from scalar and vector components.
     * @param scalar Scalar (w) component
     * @param xpos X component
     * @param ypos Y component
     * @param zpos Z component
     */
    constexpr Quaternion(float scalar, float xpos, float ypos, float zpos) noexcept :
        QQuaternion(scalar, xpos, ypos, zpos) {}
#ifndef QT_NO_VECTOR3D
    /**
     * @brief Construct from a scalar and a 3D vector.
     * @param scalar Scalar (w) component
     * @param vector 3D vector (x, y, z)
     */
    constexpr Quaternion(float scalar, const QVector3D &vector) noexcept :
        QQuaternion(scalar, vector) {}
#endif
#ifndef QT_NO_VECTOR4D
    /**
     * @brief Construct from a 4D vector.
     * @param vector 4D vector (x, y, z, w)
     */
    constexpr explicit Quaternion(const QVector4D &vector) noexcept :
        QQuaternion(vector) {}
#endif

    /**
     * @brief Format this quaternion as a comma-separated string.
     * @return String of the form "x, y, z, scalar"
     */
    QString toString() const { return QString("%1, %2, %3, %4").arg(x()).arg(y()).arg(z()).arg(scalar()); }

    /**
     * @brief Parse a quaternion from a comma-separated string.
     * @param value String of the form "x, y, z, scalar"
     * @return Parsed Quaternion
     */
    static Quaternion fromString(const QString& value);
};

#endif // QUATERNION_H
