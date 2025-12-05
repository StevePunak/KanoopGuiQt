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
#include <Kanoop/entitymetadata.h>

class LIBKANOOPGUI_EXPORT TableHeader
{
public:
    TableHeader() : _type(0), _orientation(Qt::Horizontal) {}
    TableHeader(int type, Qt::Orientation orientation = Qt::Vertical);
    TableHeader(int type, const QString& text, Qt::Orientation orientation = Qt::Vertical);

    int type() const { return _type; }
    QString text() const { return _text; }
    void setText(const QString& text) { _text = text; }

    Qt::Orientation orientation() const { return _orientation; }

    QColor columnTextColor() const { return _columnTextColor; }
    void setColumnTextColor(const QColor& value) { _columnTextColor = value; }

    bool isVisible() const { return _visible; }
    void setVisible(bool value) { _visible = value; }

    EntityMetadata entityMetadata() const { return _entityMetadata; }
    EntityMetadata& entityMetadataRef() { return _entityMetadata; }
    void setEntityMetadata(const EntityMetadata& value) { _entityMetadata = value; }

    QVariant data(int role) const { return _entityMetadata.data(role); }
    void setData(const QVariant& value, int role) { _entityMetadata.setData(value, role); }

    QUuid uuid() const { return _entityMetadata.data(KANOOP::UUidRole).toUuid(); }

    bool isValid() const { return _type != 0; }

    class List : public QList<TableHeader>
    {
    public:
        void setHeaderVisible(int type, bool visible)
        {
            auto it = std::find_if(begin(), end(), [type](TableHeader& header) { return header.type() == type; });
            if(it != end()) {
                (*it).setVisible(visible);
            }
        }

        TableHeader findByName(const QString& text) const
        {
            TableHeader result;
            auto it = std::find_if(constBegin(), constEnd(), [text](const TableHeader& header) { return header.text() == text; });
            if(it != constEnd()) {
                result = *it;
            }
            return result;
        }
    };

    class IntMap : public QMap<int, TableHeader>
    {
    public:
        List toSortedList() const
        {
            List result;
            QList<int> cols = keys();
            std::sort(cols.begin(), cols.end());
            for(int col : cols) {
                result.append(this->value(col));
            }
            return result;
        }

        void setTextColorForType(int type, const QColor& color)
        {
            auto it = std::find_if(begin(), end(), [type](TableHeader& header) { return header.type() == type; });
            if(it != end()) {
                TableHeader& header = *it;
                header.setColumnTextColor(color);
            }
        }
        void setHeaderVisible(int type, bool visible)
        {
            auto it = std::find_if(begin(), end(), [type](TableHeader& header) { return header.type() == type; });
            if(it != end()) {
                (*it).setVisible(visible);
            }
        }
        void setEntityMetadataForType(int type, const EntityMetadata& metadata)
        {
            auto it = std::find_if(begin(), end(), [type](TableHeader& header) { return header.type() == type; });
            if(it != end()) {
                TableHeader& header = *it;
                header.setEntityMetadata(metadata);
            }
        }
    };

    static QString typeToString(int type) { return _TableHeaderTypeToStringMap.value(type); }

private:
    int _type = 0;
    QString _text;
    Qt::Orientation _orientation;
    QColor _columnTextColor;
    bool _visible = true;
    EntityMetadata _entityMetadata;

    class TableHeaderTypeToStringMap : public QMap<int, QString>
    {};


    static const TableHeaderTypeToStringMap _TableHeaderTypeToStringMap;
};

#endif // TABLEHEADER_H
