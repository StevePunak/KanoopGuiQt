#include "columnsettingsdialog.h"
#include "ui_columnsettingsdialog.h"

ColumnSettingsDialog::ColumnSettingsDialog(const TableHeader::List& headers, QWidget *parent) :
    Dialog(parent),
    ui(new Ui::ColumnSettingsDialog),
    _headers(headers)
{
    ui->setupUi(this);

    performLayout();

    setApplyEnabled(false);

    setDefaultSize(200, 200);

    setWindowTitle("Column Settings");

    ui->listColumns->setEnabled(true);
    for(const TableHeader& header : headers) {
        QListWidgetItem* item = new QListWidgetItem(header.text(), ui->listColumns, header.type());
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setCheckState(header.isVisible() ? Qt::Checked : Qt::Unchecked);
    }
}

ColumnSettingsDialog::~ColumnSettingsDialog()
{
    delete ui;
}

void ColumnSettingsDialog::okClicked()
{
    for(int row = 0;row < ui->listColumns->count();row++) {
         QListWidgetItem* item = ui->listColumns->item(row);
         _headers.setHeaderVisible(item->type(), item->checkState() == Qt::Checked);
    }
}
