/******************************************************************************************
**
** stylesheets.h
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Mon Oct 16 12:07:32 2023
**
******************************************************************************************/
#ifndef STYLESHEETS_H
#define STYLESHEETS_H

#include <QColor>
#include <QString>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT StyleSheets
{
public:
    static QString backgroundColor(const QColor& color);
    static QString foregroundColor(const QColor& color);
    static QString borderRadius(int radius);
};

#endif // STYLESHEETS_H
