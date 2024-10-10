/******************************************************************************************
**
** stylesheets.cpp
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
#include "stylesheets.h"

QString StyleSheets::backgroundColor(const QColor &color)
{
    // background-color: rgb(85, 85, 255);
    return QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue());
}

QString StyleSheets::foregroundColor(const QColor &color)
{
    // color: rgb(85, 85, 255);
    return QString("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue());
}

QString StyleSheets::borderRadius(int radius)
{
    return QString("border-radius: %1px);").arg(radius);
}
