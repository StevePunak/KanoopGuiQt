#ifndef COLUMNSETTINGSDIALOG_H
#define COLUMNSETTINGSDIALOG_H

#include <Kanoop/gui/dialog.h>

#include <tableheader.h>

namespace Ui {
class ColumnSettingsDialog;
}

class ColumnSettingsDialog : public Dialog
{
    Q_OBJECT

public:
    explicit ColumnSettingsDialog(const TableHeader::List& headers, QWidget *parent = nullptr);
    ~ColumnSettingsDialog();

    TableHeader::List headers() const { return _headers; }

private:
    virtual void validate() override {}
    virtual void okClicked() override;

    Ui::ColumnSettingsDialog *ui;
    TableHeader::List _headers;
};

#endif // COLUMNSETTINGSDIALOG_H
