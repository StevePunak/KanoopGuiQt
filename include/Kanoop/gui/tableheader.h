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

/**
 * @brief Descriptor for a single column or row header in a model/view.
 *
 * TableHeader stores the header type (an application-defined integer), display
 * text, orientation, optional text colour, and visibility flag.  An EntityMetadata
 * can also be attached for carrying additional role-keyed data.
 *
 * Two container classes are provided:
 * - TableHeader::List — a QList with name- and type-search helpers
 * - TableHeader::IntMap — a QMap<int,TableHeader> with sorted-list export
 */
class LIBKANOOPGUI_EXPORT TableHeader
{
public:
    /** @brief Default constructor — creates an invalid horizontal header. */
    TableHeader() : _type(0), _orientation(Qt::Horizontal) {}

    /**
     * @brief Construct a header with a type and optional orientation.
     * @param type Application-defined header type integer
     * @param orientation Header orientation (default Qt::Vertical)
     */
    TableHeader(int type, Qt::Orientation orientation = Qt::Vertical);

    /**
     * @brief Construct a header with a type, display text, and optional orientation.
     * @param type Application-defined header type integer
     * @param text Display text
     * @param orientation Header orientation (default Qt::Vertical)
     */
    TableHeader(int type, const QString& text, Qt::Orientation orientation = Qt::Vertical);

    /**
     * @brief Return the header type integer.
     * @return Application-defined type value
     */
    int type() const { return _type; }

    /**
     * @brief Return the display text.
     * @return Header label string
     */
    QString text() const { return _text; }

    /**
     * @brief Set the display text.
     * @param text New header label string
     */
    void setText(const QString& text) { _text = text; }

    /**
     * @brief Return the header orientation.
     * @return Qt::Horizontal or Qt::Vertical
     */
    Qt::Orientation orientation() const { return _orientation; }

    /**
     * @brief Return the text colour for cells in this column.
     * @return QColor for cell text, or invalid QColor if not set
     */
    QColor columnTextColor() const { return _columnTextColor; }

    /**
     * @brief Set the text colour for cells in this column.
     * @param value New text colour
     */
    void setColumnTextColor(const QColor& value) { _columnTextColor = value; }

    /**
     * @brief Return whether this header (and its column) is visible.
     * @return true if visible
     */
    bool isVisible() const { return _visible; }

    /**
     * @brief Set header (and column) visibility.
     * @param value true to show, false to hide
     */
    void setVisible(bool value) { _visible = value; }

    /**
     * @brief Return the EntityMetadata attached to this header.
     * @return Associated EntityMetadata
     */
    EntityMetadata entityMetadata() const { return _entityMetadata; }

    /**
     * @brief Return a mutable reference to the attached EntityMetadata.
     * @return Reference to the EntityMetadata
     */
    EntityMetadata& entityMetadataRef() { return _entityMetadata; }

    /**
     * @brief Set the EntityMetadata attached to this header.
     * @param value Metadata to associate
     */
    void setEntityMetadata(const EntityMetadata& value) { _entityMetadata = value; }

    /**
     * @brief Return data stored in the attached metadata for the given role.
     * @param role Model role key
     * @return Stored QVariant, or invalid QVariant if not set
     */
    QVariant data(int role) const { return _entityMetadata.data(role); }

    /**
     * @brief Store a value in the attached metadata under the given role.
     * @param value Value to store
     * @param role Model role key
     */
    void setData(const QVariant& value, int role) { _entityMetadata.setData(value, role); }

    /**
     * @brief Return the UUID stored in the attached metadata.
     * @return UUID, or null UUID if not set
     */
    QUuid uuid() const { return _entityMetadata.data(KANOOP::UUidRole).toUuid(); }

    /**
     * @brief Return whether this header has a non-zero type.
     * @return true if type != 0
     */
    bool isValid() const { return _type != 0; }

    /**
     * @brief A list of TableHeader objects with name and type search helpers.
     */
    class List : public QList<TableHeader>
    {
    public:
        /**
         * @brief Set visibility for the header with the given type.
         * @param type Header type to find
         * @param visible true to show, false to hide
         */
        void setHeaderVisible(int type, bool visible)
        {
            auto it = std::find_if(begin(), end(), [type](TableHeader& header) { return header.type() == type; });
            if(it != end()) {
                (*it).setVisible(visible);
            }
        }

        /**
         * @brief Find the first header with a matching display text.
         * @param text Header label to search for
         * @return Matching TableHeader, or an invalid one if not found
         */
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

    /**
     * @brief An int-keyed map of TableHeader objects with bulk-operation helpers.
     */
    class IntMap : public QMap<int, TableHeader>
    {
    public:
        /**
         * @brief Return all headers as a list sorted by key (column index).
         * @return Sorted TableHeader::List
         */
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

        /**
         * @brief Set the text colour for the header with the given type.
         * @param type Header type to find
         * @param color New text colour
         */
        void setTextColorForType(int type, const QColor& color)
        {
            auto it = std::find_if(begin(), end(), [type](TableHeader& header) { return header.type() == type; });
            if(it != end()) {
                TableHeader& header = *it;
                header.setColumnTextColor(color);
            }
        }

        /**
         * @brief Set visibility for the header with the given type.
         * @param type Header type to find
         * @param visible true to show, false to hide
         */
        void setHeaderVisible(int type, bool visible)
        {
            auto it = std::find_if(begin(), end(), [type](TableHeader& header) { return header.type() == type; });
            if(it != end()) {
                (*it).setVisible(visible);
            }
        }

        /**
         * @brief Attach EntityMetadata to the header with the given type.
         * @param type Header type to find
         * @param metadata Metadata to associate
         */
        void setEntityMetadataForType(int type, const EntityMetadata& metadata)
        {
            auto it = std::find_if(begin(), end(), [type](TableHeader& header) { return header.type() == type; });
            if(it != end()) {
                TableHeader& header = *it;
                header.setEntityMetadata(metadata);
            }
        }
    };

    /**
     * @brief Look up the string name for a header type integer.
     * @param type Header type integer
     * @return Registered name string, or empty string if not found
     */
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
