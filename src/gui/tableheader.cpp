/******************************************************************************************
**
** tableheader.cpp
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Wed Oct 11 18:17:49 2023
**
******************************************************************************************/
#include "tableheader.h"

#include <QJsonDocument>

TableHeader::TableHeader(int type, Qt::Orientation orientation) :
    _type(type), _orientation(orientation) {}

TableHeader::TableHeader(int type, const QString &text, Qt::Orientation orientation) :
    _type(type), _text(text), _orientation(orientation) {}
