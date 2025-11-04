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

        BackDelete,
        DropDownDown,
        DropDownUp,
        CloseButton,
        GreenPlus,
        Label,
        NextTrack,
        Pause,
        Play,
        PreviousTrack,
        RedMinus,
        Refresh,
        Search,
        Spinner,
        Stop,
    };

    static const int FirstUserResource = 100000;

private:
    static bool registerStandardImages();

    class StandardImageToStringMap : public KANOOP::EnumToStringMap<StandardImage>
    {
    public:
        StandardImageToStringMap()
        {
            insert(BackDelete,          "back-delete.png");
            insert(CloseButton,         "close-button.png");
            insert(DropDownDown,        "dropdown-down.png");
            insert(DropDownUp,          "dropdown-up.png");
            insert(GreenPlus,           "green-plus.png");
            insert(Label,               "label.png");
            insert(NextTrack,           "next-track.png");
            insert(Pause,               "pause.png");
            insert(Play,                "play.png");
            insert(PreviousTrack,       "prev-track.png");
            insert(RedMinus,            "red-minus.png");
            insert(Refresh,             "refresh.png");
            insert(Search,              "search.png");
            insert(Spinner,             "spinner.png");
            insert(Stop,                "stop.png");
        }
    };

    static QMap<int, QString> _registeredImages;
    static bool _standardImagesRegistered;
    static const StandardImageToStringMap _StandardImageToStringMap;
};

#endif // RESOURCES_H
