/******************************************************************************************
**
** libtessgui.h
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Mon Oct 9 11:13:45 2023
**
******************************************************************************************/
#ifndef LIBTESSGUI_H
#define LIBTESSGUI_H

#include <QtCore/qglobal.h>

/**
 * @def LIBKANOOPGUI_EXPORT
 * @brief Symbol visibility macro for KanoopGuiQt public API.
 *
 * Expands to Q_DECL_EXPORT when building the library and Q_DECL_IMPORT when
 * consuming it.  Apply to every class or function that is part of the public API.
 */
#if defined(KANOOP_QTGUI_LIBRARY)
#  define LIBKANOOPGUI_EXPORT Q_DECL_EXPORT
#else
#  define LIBKANOOPGUI_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBTESSGUI_H
