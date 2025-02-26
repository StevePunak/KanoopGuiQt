/******************************************************************************************
**
** tableheader.h
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Fri Oct 27 18:08:12 2023
**
******************************************************************************************/
#ifndef TABLEHEADER_H
#define TABLEHEADER_H
#include <QColor>
#include <QMap>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT TableHeader
{
public:
    TableHeader() : _type(0), _orientation(Qt::Horizontal) {}
    TableHeader(int type, Qt::Orientation orientation = Qt::Vertical);
    TableHeader(int type, const QString& text, Qt::Orientation orientation = Qt::Vertical);

    int type() const { return _type; }
    QString text() const { return _text; }
    Qt::Orientation orientation() const { return _orientation; }

    QColor columnTextColor() const { return _columnTextColor; }
    void setColumnTextColor(const QColor& value) { _columnTextColor = value; }

    bool isValid() const { return _type != 0; }

    class IntMap : public QMap<int, TableHeader>
    {
    public:
        void setTextColorForType(int type, const QColor& color)
        {
            auto it = find(type);
            if(it != end()) {
                TableHeader& header = *it;
                header.setColumnTextColor(color);
            }
        }
    };

    class StringMap : public QMap<QString, TableHeader>
    {
    public:
        void setTextColorForType(int type, const QColor& color)
        {
            for(TableHeader& header : *this) {
                if(header.type() == type) {
                    header.setColumnTextColor(color);
                }
            }
        }
    };

    class List : public QList<TableHeader> {};

    static QString typeToString(int type) { return _TableHeaderTypeToStringMap.value(type); }

private:
    int _type;
    QString _text;
    Qt::Orientation _orientation;
    QColor _columnTextColor;

    class TableHeaderTypeToStringMap : public QMap<int, QString>
    {};


    static const TableHeaderTypeToStringMap _TableHeaderTypeToStringMap;
};

#endif // TABLEHEADER_H
