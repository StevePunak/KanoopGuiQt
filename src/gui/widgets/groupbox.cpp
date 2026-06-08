#include "widgets/groupbox.h"


GroupBox::GroupBox(QWidget *parent) :
    QGroupBox{parent}
{

}

GroupBox::GroupBox(const QString& title, QWidget* parent) :
    QGroupBox(title, parent)
{

}

#include "Kanoop/gui/widgets/moc_groupbox.cpp"
