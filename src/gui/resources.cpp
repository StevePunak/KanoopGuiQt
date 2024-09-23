/******************************************************************************************
**
** resources.cpp
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
** Created: Sun Oct 22 17:41:53 2023
**
******************************************************************************************/
#include "resources.h"

#include <Kanoop/log.h>

#include <QIcon>
#include <QMutex>

#include <Kanoop/pathutil.h>

QMap<int, QString> Resources::_registeredImages;

void Resources::registerImage(int entityMetadataType, const QString &resourcePath)
{
    static QMutex _lock;
    _lock.lock();
    if(_registeredImages.contains(entityMetadataType) && _registeredImages.value(entityMetadataType) != resourcePath) {
        Log::logText(LVL_WARNING, QString("Replacing metadata type %1").arg(resourcePath));
    }
    _registeredImages.insert(entityMetadataType, resourcePath);
    _lock.unlock();

}

QIcon Resources::getIcon(int entityMetadataType)
{
    QIcon result = QIcon(getPixmap(entityMetadataType));
    if(result.isNull()) {
        Log::logText(LVL_DEBUG, QString("No icon found for %1").arg(entityMetadataType));
    }
    return result;
}

QPixmap Resources::getPixmap(int entityMetadataType)
{
    QPixmap result;
    QString resourcePath = _registeredImages.value(entityMetadataType);
    if(resourcePath.isEmpty() == false) {
        result = QPixmap(resourcePath);
        if(result.isNull()) {
            Log::logText(LVL_DEBUG, QString("No registered resource found for %1").arg(entityMetadataType));
        }
    }
    else {
        Log::logText(LVL_DEBUG, QString("No resource Pixmap from %1").arg(resourcePath));
    }
    return result;
}
