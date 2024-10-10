/******************************************************************************************
**
** resources.h
**
** Author:  Stephen Punak
** Created: Fri Oct 27 18:08:12 2023
**
******************************************************************************************/
#ifndef RESOURCES_H
#define RESOURCES_H
#include <QIcon>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT Resources
{
public:
    static void registerImage(int entityMetadataType, const QString& resourcePath);

    static QIcon getIcon(int entityMetadataType);
    static QPixmap getPixmap(int entityMetadataType);

private:
    static QMap<int, QString> _registeredImages;
};

#endif // RESOURCES_H
