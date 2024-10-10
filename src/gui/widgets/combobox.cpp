#include "widgets/combobox.h"

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
