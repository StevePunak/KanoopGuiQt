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

/**
 * @brief KanoopGuiQt namespace containing GUI-specific enumerations and helpers.
 */
namespace GUI
{

/**
 * @brief Drag-and-drop gesture state.
 */
enum DragState
{
    NotDragging,    ///< No drag in progress
    MaybeDrag,      ///< Mouse pressed but threshold not reached
    Dragging        ///< Active drag in progress
};

/**
 * @brief Top-level window state.
 */
enum WindowState
{
    NormalState,    ///< Window is in normal (restored) state
    Closed,         ///< Window has been closed
    Minimized,      ///< Window is minimized
    Maximized,      ///< Window is maximized
};

/**
 * @brief Title-bar button visibility flags.
 */
enum TitlebarButtonOption
{
    NoButton = 0,

    MaximizeButton =    0x01,   ///< Show the maximize button
    MinimizeButton =    0x02,   ///< Show the minimize button
    CloseButton =       0x04,   ///< Show the close button
};
Q_DECLARE_FLAGS(TitlebarButtonOptions, TitlebarButtonOption)
Q_DECLARE_OPERATORS_FOR_FLAGS(TitlebarButtonOptions)

/**
 * @brief Register KanoopGuiQt metatypes with Qt's type system.
 *
 * Call this once at application startup (e.g. from main()) before using
 * GUI types in signals, slots, or QVariant containers.
 */
LIBKANOOPGUI_EXPORT void registerMetatypes();
} // namespace GUI


#endif // GUITYPES_H
