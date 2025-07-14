#include "widgets/combobox.h"

#include <QKeyEvent>
#include <QStandardItemModel>

ComboBox::ComboBox(QWidget *parent) :
    QComboBox(parent)
{
}

void ComboBox::setBold(bool bold)
{
    QFont f = font();
    f.setBold(bold);
    setFont(f);
}

void ComboBox::setRowBold(int row, bool bold)
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(QComboBox::model());
    if(model == nullptr) {
        return;
    }
    QStandardItem* item = model->item(row);
    if(item != nullptr) {
        QFont font = item->font();
        font.setBold(bold);
        item->setFont(font);
    }
}

void ComboBox::keyPressEvent(QKeyEvent* event)
{
    QComboBox::keyPressEvent(event);
    if(event->key() == Qt::Key_Return) {
        emit accept();
    }
}

void ComboBox::focusOutEvent(QFocusEvent* event)
{
    QComboBox::focusOutEvent(event);
    emit lostFocus();
}

