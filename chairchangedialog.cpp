#include "chairchangedialog.h"
#include "ui_chairchangedialog.h"


ChairChangeDialog::ChairChangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChairChangeDialog)
{
    ui->setupUi(this);
}

void ChairChangeDialog::editItem(QTableWidget* table_,int row_)
{
    this->table = table_;
    this->row = row_;
    if(row != -1)
    {
        ui->FullChairName->setText(this->table->item(row,0)->text());
        ui->ShortChairName->setText(this->table->item(row,1)->text());
    }
}

ChairChangeDialog::~ChairChangeDialog()
{
    delete ui;
}

void ChairChangeDialog::on_OKButton_clicked()
{
    if(row != -1)
    {
        this->table->item(row,0)->setText(ui->FullChairName->text());
        this->table->item(row,1)->setText(ui->ShortChairName->text());
    }
    else
    {
        table->setRowCount(table->rowCount()+1);
        this->table->setItem(table->rowCount()-1,0,new QTableWidgetItem(ui->FullChairName->text()));
        this->table->setItem(table->rowCount()-1,1,new QTableWidgetItem(ui->ShortChairName->text()));
    }
    this->close();
}

void ChairChangeDialog::on_CancelButton_clicked()
{
    this->close();
}
