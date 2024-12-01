#include "vector3d.h"
#include <QStringList>

Vector3D Vector3D::fromString(const QString& value)
{
    Vector3D result;
    QStringList parts = value.split(',', Qt::SkipEmptyParts);
    if(parts.count() == 3) {
        double x = parts.at(0).trimmed().toDouble();
        double y = parts.at(1).trimmed().toDouble();
        double z = parts.at(2).trimmed().toDouble();
        result = Vector3D(x, y, z);
    }
    return result;
}
