#ifndef VECTOR3D_H
#define VECTOR3D_H
#include <QVector3D>

/**
 * @brief QVector3D subclass with string serialization support.
 *
 * Mirrors all QVector3D constructors and adds toString() / fromString()
 * for human-readable "x, y, z" serialization.
 */
class Vector3D : public QVector3D
{
public:
    /** @brief Default constructor — creates a zero vector. */
    constexpr Vector3D() noexcept :
        QVector3D() {}

    /**
     * @brief Construct with a Qt initialization sentinel.
     * @param initialization Qt::Initialization value
     */
    explicit Vector3D(Qt::Initialization initialization) noexcept :
        QVector3D(initialization) {}

    /**
     * @brief Construct with explicit x, y, z components.
     * @param xpos X component
     * @param ypos Y component
     * @param zpos Z component
     */
    constexpr Vector3D(float xpos, float ypos, float zpos) noexcept :
        QVector3D(xpos, ypos, zpos) {}

    /**
     * @brief Construct from a QPoint (z = 0).
     * @param point Source point
     */
    constexpr explicit Vector3D(QPoint point) noexcept :
        QVector3D(point) {}

    /**
     * @brief Construct from a QPointF (z = 0).
     * @param point Source point
     */
    constexpr explicit Vector3D(QPointF point) noexcept :
        QVector3D(point) {}

#ifndef QT_NO_VECTOR2D
    /**
     * @brief Construct from a QVector2D (z = 0).
     * @param vector Source 2D vector
     */
    constexpr explicit Vector3D(QVector2D vector) noexcept :
        QVector3D(vector) {}

    /**
     * @brief Construct from a QVector2D with an explicit z component.
     * @param vector Source 2D vector
     * @param zpos Z component
     */
    constexpr Vector3D(QVector2D vector, float zpos) noexcept :
        QVector3D(vector, zpos) {}
#endif

#ifndef QT_NO_VECTOR4D
    /**
     * @brief Construct from a QVector4D (w is discarded).
     * @param vector Source 4D vector
     */
    constexpr explicit Vector3D(QVector4D vector) noexcept :
        QVector3D(vector) {}
#endif

    /**
     * @brief Serialize the vector to a human-readable string.
     * @return String of the form "x, y, z"
     */
    QString toString() const { return QString("%1, %2, %3").arg(x()).arg(y()).arg(z()); }

    /**
     * @brief Deserialize a vector from a "x, y, z" string.
     * @param value String to parse
     * @return Parsed Vector3D
     */
    static Vector3D fromString(const QString& value);

};

#endif // VECTOR3D_H
