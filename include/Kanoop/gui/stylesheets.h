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

/**
 * @brief Static helpers for generating common Qt stylesheet fragments.
 *
 * Each method returns a stylesheet snippet string that can be concatenated
 * or passed directly to QWidget::setStyleSheet().
 */
class LIBKANOOPGUI_EXPORT StyleSheets
{
public:
    /**
     * @brief Build a background-color stylesheet fragment.
     * @param color Background colour
     * @return Stylesheet string, e.g. "background-color: #rrggbb;"
     */
    static QString backgroundColor(const QColor& color);

    /**
     * @brief Build a color (foreground) stylesheet fragment.
     * @param color Foreground colour
     * @return Stylesheet string, e.g. "color: #rrggbb;"
     */
    static QString foregroundColor(const QColor& color);

    /**
     * @brief Build a border-radius stylesheet fragment.
     * @param radius Border radius in pixels
     * @return Stylesheet string, e.g. "border-radius: 4px;"
     */
    static QString borderRadius(int radius);
};

#endif // STYLESHEETS_H
