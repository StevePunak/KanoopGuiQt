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

/**
 * @brief Central registry mapping integer IDs to Qt resource images.
 *
 * Resources maintains a map of integer IDs to Qt resource paths and provides
 * factory methods for retrieving QIcon and QPixmap instances by ID.
 *
 * Standard images (play, pause, spinner, etc.) are pre-registered under the
 * StandardImage enum values.  Application-specific images should use IDs
 * starting at FirstUserResource to avoid collisions.
 */
class LIBKANOOPGUI_EXPORT Resources
{
public:
    /**
     * @brief Register a Qt resource path under an integer ID.
     * @param id Integer ID to associate with the image
     * @param resourcePath Qt resource path (e.g. ":/icons/play.png")
     */
    static void registerImage(int id, const QString& resourcePath);

    /**
     * @brief Return a QIcon for the given image ID.
     * @param id Registered image ID
     * @return QIcon loaded from the registered resource path
     */
    static QIcon getIcon(int id);

    /**
     * @brief Return a QPixmap for the given image ID.
     * @param id Registered image ID
     * @return QPixmap loaded from the registered resource path
     */
    static QPixmap getPixmap(int id);

    /**
     * @brief Built-in standard image identifiers.
     */
    enum StandardImage
    {
        UnknownImage = 0,   ///< Sentinel value for an unknown image

        BackDelete,         ///< Back-delete / clear icon
        CheckBox,           ///< Checkbox icon
        DropDownDown,       ///< Drop-down arrow pointing down
        DropDownUp,         ///< Drop-down arrow pointing up
        CloseButton,        ///< Close / X button icon
        GreenPlus,          ///< Green plus / add icon
        Label,              ///< Label icon
        NextTrack,          ///< Next track / skip-forward icon
        Pause,              ///< Pause icon
        Play,               ///< Play icon
        PreviousTrack,      ///< Previous track / skip-back icon
        RedMinus,           ///< Red minus / remove icon
        Refresh,            ///< Refresh / reload icon
        Search,             ///< Search / magnifier icon
        Spinner,            ///< Spinner / busy indicator icon
        Stop,               ///< Stop icon
    };

    /** @brief First ID safe for application-defined resources (avoid collisions with StandardImage). */
    static const int FirstUserResource = 100000;

private:
    /** @brief Register all StandardImage entries; called lazily on first use. */
    static bool registerStandardImages();

    class StandardImageToStringMap : public KANOOP::EnumToStringMap<StandardImage>
    {
    public:
        StandardImageToStringMap()
        {
            insert(BackDelete,          "back-delete.png");
            insert(CheckBox,            "checkbox.png");
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
