#ifndef HEADERSTATE_H
#define HEADERSTATE_H
#include <Kanoop/serialization/iserializabletojson.h>
#include <Kanoop/serialization/ideserializablefromjson.h>
#include <Kanoop/serialization/serializablejsonlist.h>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief Serializable snapshot of a QHeaderView's section sizes and visibility.
 *
 * HeaderState captures per-section metadata (index, label, pixel width, and visibility)
 * so that a header view layout can be saved to JSON and restored later.
 */
class LIBKANOOPGUI_EXPORT HeaderState : public ISerializableToJson,
                                        public IDeserializableFromJson
{
public:
    /** @brief Virtual destructor. */
    virtual ~HeaderState() {}

    /**
     * @brief Serialize this header state to a JSON byte array.
     * @return JSON-encoded representation
     */
    virtual QByteArray serializeToJson() const override;

    /**
     * @brief Deserialize a header state from a JSON byte array.
     * @param json JSON-encoded representation
     */
    virtual void deserializeFromJson(const QByteArray &json) override;

    /**
     * @brief Describes the saved state of a single header section.
     */
    class SectionState : public ISerializableToJsonObject,
                         public IDeserializableFromJsonObject
    {
    public:
        /** @brief Default constructor — creates an invalid section state. */
        SectionState() :
            _section(0), _size(100) {}

        /**
         * @brief Construct with explicit section attributes.
         * @param section Logical section index
         * @param text Header label text
         * @param size Section pixel width
         * @param visible Whether the section is visible
         */
        SectionState(int section, const QString& text, int size, bool visible) :
            _section(section), _text(text), _size(size), _visible(visible)  {}

        /** @brief Virtual destructor. */
        virtual ~SectionState() {}

        /** @brief Serialize this section state to a QJsonObject. */
        virtual QJsonObject serializeToJsonObject() const override;
        /** @brief Deserialize this section state from a QJsonObject. */
        virtual void deserializeFromJsonObject(const QJsonObject &jsonObject) override;

        /**
         * @brief Return the logical section index.
         * @return Section index
         */
        int section() const { return _section; }

        /**
         * @brief Return the header label text.
         * @return Header text string
         */
        QString text() const { return _text; }

        /**
         * @brief Return the section pixel width.
         * @return Width in pixels
         */
        int size() const { return _size; }

        /**
         * @brief Return whether the section is visible.
         * @return true if visible
         */
        bool isVisible() const { return _visible; }

        /**
         * @brief Return whether this section state has a non-empty label.
         * @return true if the text is non-empty
         */
        bool isValid() const { return _text.isEmpty() == false; }

        /**
         * @brief A serializable list of SectionState objects.
         */
        class List : public SerializableJsonList<SectionState>
        {
        public:
            /**
             * @brief Find the SectionState for a given section index.
             * @param section Logical section index to find
             * @return Matching SectionState, or an invalid one if not found
             */
            SectionState findForSection(int section)
            {
                SectionState result;
                const_iterator it = std::find_if(constBegin(), constEnd(), [section](const SectionState& s) { return s.section() == section; } );
                if(it != constEnd()) {
                    result = *it;
                }
                return result;
            }
        };

    private:
        int _section = 0;
        QString _text;
        int _size = 100;
        bool _visible = true;
    };

    /**
     * @brief Record a section in this header state.
     * @param section Logical section index
     * @param text Header label text
     * @param size Section pixel width
     * @param visible Whether the section is visible
     */
    void addSection(int section, const QString& text, int size, bool visible);

    /**
     * @brief Return the saved state for a given section index.
     * @param section Logical section index
     * @return Matching SectionState, or an invalid one if not found
     */
    SectionState getSection(int section) { return _sections.findForSection(section); }

private:
    SectionState::List _sections;
};

#endif // HEADERSTATE_H
