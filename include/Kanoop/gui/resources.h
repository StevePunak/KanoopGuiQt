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
#include <Kanoop/kanoopcommon.h>

class LIBKANOOPGUI_EXPORT Resources
{
public:
    static void registerImage(int id, const QString& resourcePath);

    static QIcon getIcon(int id);
    static QPixmap getPixmap(int id);

    enum StandardImage
    {
        UnknownImage = 0,

        CloseButton,
        Spinner,
    };

    static const int FirstUserResource = 100000;

private:
    static bool registerStandardImages();

    class StandardImageToStringMap : public KANOOP::EnumToStringMap<StandardImage>
    {
    public:
        StandardImageToStringMap()
        {
            insert(CloseButton,         "close-button.png");
            insert(Spinner,             "spinner.png");
        }
    };

    static QMap<int, QString> _registeredImages;
    static bool _standardImagesRegistered;
    static const StandardImageToStringMap _StandardImageToStringMap;
};

#endif // RESOURCES_H
