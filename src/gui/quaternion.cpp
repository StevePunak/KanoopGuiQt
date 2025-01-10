#include "quaternion.h"


Quaternion Quaternion::fromString(const QString& value)
{
    Quaternion result;
    QStringList parts = value.split(',', Qt::SkipEmptyParts);
    if(parts.count() == 4) {
        double x = parts.at(0).trimmed().toDouble();
        double y = parts.at(1).trimmed().toDouble();
        double z = parts.at(2).trimmed().toDouble();
        double scalar = parts.at(3).trimmed().toDouble();
        result = Quaternion(scalar, x, y, z);
    }
    return result;
}
