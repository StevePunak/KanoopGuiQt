#include "utility/stylesheet.h"
#include <QAbstractItemView>
#include <QGradient>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QSlider>
#include <QStatusBar>
#include <QTableView>
#include <QTextEdit>
#include <QTreeView>
#include <QCheckBox>
#include <QComboBox>
#include <QMenu>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QRadioButton>
#include <QScrollBar>
#include <QToolBar>
#include <QGroupBox>
#include <QWizard>


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
    output << _typeName;
    if(_subControl.isEmpty() == false) {
        output << "::" << _subControl;
    }
    for(StyleSheetPseudoState ps : _pseudoStates) {
        output << ':' << StyleSheetStrings::getPseudoStateString(ps);
    }
    output << " {";
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


template<typename T>
void StyleSheet<T>::setRadialGradient(double cx, double cy, double radius, double fx, double fy,
                                      const QGradientStops& stops)
{
    QString s = QString("qradialgradient(cx:%1, cy:%2, radius:%3, fx:%4, fy:%5")
        .arg(cx).arg(cy).arg(radius).arg(fx).arg(fy);
    for(const QGradientStop& stop : stops) {
        const QColor& c = stop.second;
        s += QString(", stop:%1 rgba(%2,%3,%4,%5)")
            .arg(stop.first).arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
    }
    s += QLatin1Char(')');
    setGradient(s);
}

template<typename T>
void StyleSheet<T>::setBorder(const QString& topLeft, const QString& bottomRight)
{
    setProperty(SP_BorderTop,    topLeft);
    setProperty(SP_BorderLeft,   topLeft);
    setProperty(SP_BorderBottom, bottomRight);
    setProperty(SP_BorderRight,  bottomRight);
}

template<typename T>
void StyleSheet<T>::setBorder(int widthPx, const QColor& topLeft, const QColor& bottomRight)
{
    auto make = [widthPx](const QColor& c) {
        return QString("%1px solid rgba(%2,%3,%4,%5)")
            .arg(widthPx).arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
    };
    setBorder(make(topLeft), make(bottomRight));
}

template<typename T>
void StyleSheet<T>::setSubControl(StyleSheetSubControl value)
{
    _subControl = StyleSheetStrings::getSubControlString(value);
}

template class StyleSheet<QWidget>;
template class StyleSheet<QAbstractItemView>;
template class StyleSheet<QFrame>;
template class StyleSheet<QGroupBox>;
template class StyleSheet<QLabel>;
template class StyleSheet<QPushButton>;
template class StyleSheet<QListView>;
template class StyleSheet<QTreeView>;
template class StyleSheet<QTableView>;
template class StyleSheet<QTextEdit>;
template class StyleSheet<QLineEdit>;
template class StyleSheet<QStatusBar>;
template class StyleSheet<QToolBar>;
template class StyleSheet<QMenu>;
template class StyleSheet<QRadioButton>;
template class StyleSheet<QPlainTextEdit>;
template class StyleSheet<QMenuBar>;
template class StyleSheet<QCheckBox>;
template class StyleSheet<QComboBox>;
template class StyleSheet<QTabWidget>;
template class StyleSheet<QScrollBar>;
template class StyleSheet<QSlider>;
template class StyleSheet<QTabBar>;
template class StyleSheet<QWizard>;
template class StyleSheet<QWizardPage>;

