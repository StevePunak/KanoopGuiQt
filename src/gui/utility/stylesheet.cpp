#include "utility/stylesheet.h"
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>


template<typename T>
void StyleSheet<T>::setProperty(StyleSheetProperty property, const QString& value)
{
    _properties.insert(property, value);
}

template<typename T>
void StyleSheet<T>::setProperty(StyleSheetProperty property, const QColor& value)
{
    _properties.insert(property, value.name(QColor::HexArgb));
}

template<typename T>
void StyleSheet<T>::setPropertyPixels(StyleSheetProperty property, int value)
{
    _properties.insert(property, QString("%1px").arg(value));
}

template<typename T>
QString StyleSheet<T>::toString() const
{
    QString result;
    QTextStream output(&result);
    output << QString("%1 {").arg(_typeName);
    QList<StyleSheetProperty> keys = _properties.keys();
    for(int i = 0; i < keys.count();i++) {
        StyleSheetProperty p = keys.at(i);
        output << ' ' << StyleSheetStrings::getPropertyString(p) << ": " << _properties.value(p);
        if(i < keys.count() - 1) {
            output << ';';
        }
    }
    output << " }";
    return result;
}


template class StyleSheet<QWidget>;
template class StyleSheet<QFrame>;
template class StyleSheet<QLabel>;
template class StyleSheet<QPushButton>;
template class StyleSheet<QTextEdit>;
template class StyleSheet<QLineEdit>;
