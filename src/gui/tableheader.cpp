/******************************************************************************************
**
** tableheader.cpp
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
** Created: Wed Oct 11 18:17:49 2023
**
******************************************************************************************/
#include "tableheader.h"

#include <QJsonDocument>

const TableHeader::TableHeaderTypeToStringMap TableHeader::_TableHeaderTypeToStringMap;

TableHeader::TableHeader(int type, Qt::Orientation orientation) :
    _type(type), _text(_TableHeaderTypeToStringMap.value(type)), _orientation(orientation) {}

TableHeader::TableHeader(int type, const QString &text, Qt::Orientation orientation) :
    _type(type), _text(text), _orientation(orientation)
{
    if(_text.isEmpty()) {
        _text = _TableHeaderTypeToStringMap.value(type);
    }
}
