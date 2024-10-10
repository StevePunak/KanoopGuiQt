/******************************************************************************************
**
** stylesheets.h
**
** Copyright (C) Tesseract Engineering, Inc - All Rights Reserved
**
** This source code is protected under international copyright law.  All rights
** reserved and protected by the copyright holders.
**
** This file is confidential and only available to authorized individuals with the
** permission of the copyright holders.  If you encounter this file and do not have
** permission, please contact the copyright holders and delete this file.
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
};

#endif // STYLESHEETS_H
