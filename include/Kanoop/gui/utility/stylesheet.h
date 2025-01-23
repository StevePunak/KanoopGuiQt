#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <Kanoop/kanoopcommon.h>
#include <Kanoop/gui/utility/stylesheettypes.h>
#include <Kanoop/gui/libkanoopgui.h>

#include <QWidget>

template <typename T>
class LIBKANOOPGUI_EXPORT StyleSheet
{
public:
    static_assert(std::is_base_of<QWidget, T>::value, "Templatized class type must be subclass of QWidget");

    StyleSheet()
    {
        T* t = new T();
        _typeName = t->metaObject()->className();
        delete t;
    }

    void setProperty(StyleSheetProperty property, const QString& value);
    void setProperty(StyleSheetProperty property, const QColor& value);
    void setPropertyPixels(StyleSheetProperty property, int value);
    QString toString() const;


    QString _typeName;

    QMap<StyleSheetProperty, QString> _properties;
};

#endif // STYLESHEET_H
