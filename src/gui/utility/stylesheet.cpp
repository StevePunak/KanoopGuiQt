#include "utility/stylesheet.h"
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QStatusBar>
#include <QTableView>
#include <QTextEdit>
#include <QTreeView>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qplaintextedit.h>
#include <qradiobutton.h>
#include <qscrollbar.h>
#include <qtoolbar.h>


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
    if(_pseudoState != PS_Invalid) {
        output << ':' << StyleSheetStrings::getPseudoStateString(_pseudoState);
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


template class StyleSheet<QWidget>;
template class StyleSheet<QFrame>;
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
template class StyleSheet<QTabBar>;

