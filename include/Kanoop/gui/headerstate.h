#ifndef HEADERSTATE_H
#define HEADERSTATE_H
#include <Kanoop/serialization/iserializabletojson.h>
#include <Kanoop/serialization/ideserializablefromjson.h>
#include <Kanoop/serialization/serializablejsonlist.h>

class HeaderState : public ISerializableToJson,
                    public IDeserializableFromJson
{
public:
    virtual QByteArray serializeToJson() const override;
    virtual void deserializeFromJson(const QByteArray &json) override;

    class SectionState : public ISerializableToJsonObject,
                         public IDeserializableFromJsonObject
    {
    public:
        SectionState() :
            _section(0), _size(100) {}
        SectionState(int section, const QString& text, int size) :
            _section(section), _text(text), _size(size)  {}

        virtual QJsonObject serializeToJsonObject() const override;
        virtual void deserializeFromJsonObject(const QJsonObject &jsonObject) override;

        int section() const { return _section; }
        QString text() const { return _text; }
        int size() const { return _size; }

        bool isValid() const { return _text.isEmpty() == false; }

        class List : public SerializableJsonList<SectionState>
        {
        public:
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
        int _section;
        QString _text;
        int _size;
    };

    void addSection(int section, const QString& text, int size);
    SectionState getSection(int section) { return _sections.findForSection(section); }

private:
    SectionState::List _sections;
};

#endif // HEADERSTATE_H
