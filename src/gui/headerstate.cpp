#include "headerstate.h"

#include <QJsonDocument>

QByteArray HeaderState::serializeToJson() const
{
    QJsonDocument doc;

    QJsonArray array = _sections.serializeToJsonArray();
    QJsonObject rootObject;
    rootObject["sections"] = array;

    doc.setObject(rootObject);
    return doc.toJson();
}

void HeaderState::deserializeFromJson(const QByteArray &json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json);
    _sections.deserializeFromJsonArray(doc["sections"].toArray());
}

void HeaderState::addSection(int section, const QString &text, int size)
{
    _sections.append(SectionState(section, text, size));
}

QJsonObject HeaderState::SectionState::serializeToJsonObject() const
{
    QJsonObject result;
    result["section"] = _section;
    result["text"] = _text;
    result["size"] = _size;
    return result;
}

void HeaderState::SectionState::deserializeFromJsonObject(const QJsonObject &jsonObject)
{
    _section = jsonObject["section"].toInt();
    _text = jsonObject["text"].toString();
    _size = jsonObject["size"].toInt();
}
