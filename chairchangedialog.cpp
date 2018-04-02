#include "chairchangedialog.h"
#include "ui_chairchangedialog.h"


ChairChangeDialog::ChairChangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChairChangeDialog)
{
    ui->setupUi(this);
}

void ChairChangeDialog::setItem(QTableWidgetItem* fNameitem_,QTableWidgetItem* sNameitem_)
{
    this->fNameitem = fNameitem_;
    this->sNameitem = sNameitem_;
    ui->FullChairName->setText(this->fNameitem->text());
    ui->ShortChairName->setText(this->sNameitem->text());
}

ChairChangeDialog::~ChairChangeDialog()
{
    delete ui;
}

void ChairChangeDialog::on_OKButton_clicked()
{
    fNameitem->setText(ui->FullChairName->text());
    sNameitem->setText(ui->ShortChairName->text());
    this->close();
}

void ChairChangeDialog::on_CancelButton_clicked()
{
    this->close();
}
