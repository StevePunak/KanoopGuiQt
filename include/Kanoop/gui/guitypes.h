/******************************************************************************************
**
** guitypes.h
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Fri Oct 13 13:08:02 2023
**
******************************************************************************************/
#ifndef GUITYPES_H
#define GUITYPES_H
#include <Qt>
#include <Kanoop/gui/libkanoopgui.h>

namespace GUI
{
enum DragState
{
    NotDragging,
    MaybeDrag,
    Dragging
};

enum WindowState
{
    NormalState,
    Closed,
    Minimized,
    Maximized,
};

enum TitlebarButtonOption
{
    NoButton = 0,

    MaximizeButton =    0x01,
    MinimizeButton =    0x02,
    CloseButton =       0x04,
};
Q_DECLARE_FLAGS(TitlebarButtonOptions, TitlebarButtonOption)
Q_DECLARE_OPERATORS_FOR_FLAGS(TitlebarButtonOptions)

LIBKANOOPGUI_EXPORT void registerMetatypes();
} // namespace GUI


#endif // GUITYPES_H
