/******************************************************************************************
**
** stylesheets.cpp
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Mon Oct 16 12:07:32 2023
**
******************************************************************************************/
#include "stylesheets.h"

QString StyleSheets::backgroundColor(const QColor &color)
{
    // background-color: rgb(85, 85, 255);
    return QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue());
}

QString StyleSheets::foregroundColor(const QColor &color)
{
    // color: rgb(85, 85, 255);
    return QString("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue());
}

QString StyleSheets::borderRadius(int radius)
{
    return QString("border-radius: %1px);").arg(radius);
}
