#include "catalogsform.h"
#include "ui_catalogsform.h"

CatalogsForm::CatalogsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CatalogsForm)
{
    ui->setupUi(this);
}

CatalogsForm::~CatalogsForm()
{
    delete ui;
}

void CatalogsForm::loadCatalogs(XMLParser *parser_)
{
    this->parser = parser_;
    loadChairs();
}

void CatalogsForm::loadChairs()
{
    ui->CatalogsChairsTable->setRowCount(parser->getChairs()->size());
    int j = 0;
    for (QHash<int,Items::ChairObj>::iterator i = parser->getChairs()->begin(); i != parser->getChairs()->end(); ++i)
    {
        ui->CatalogsChairsTable->setItem(j,0, new QTableWidgetItem(i.value().fullName));
        ui->CatalogsChairsTable->setItem(j,1, new QTableWidgetItem(i.value().shortName));
        j++;
    }
}

void CatalogsForm::on_CatalogsChairsTable_cellDoubleClicked(int row, int column)
{
    ChairChangeDialog* chd = new ChairChangeDialog;
    chd->editItem(ui->CatalogsChairsTable,row);
    chd->show();
}

void CatalogsForm::on_AddChairButton_clicked()
{
    ChairChangeDialog* chd = new ChairChangeDialog;
    chd->editItem(ui->CatalogsChairsTable,-1);
    chd->show();
}

void CatalogsForm::on_DeleteChairButton_clicked()
{
    if(ui->CatalogsChairsTable->currentRow() != -1)
    {
        auto answer = QMessageBox::warning(this,tr("Подтверждение удаления"),"Вы действительно хотите удалить кафедру - \""+ui->CatalogsChairsTable->item(ui->CatalogsChairsTable->currentRow(),0)->text()+"\"?",
                                           QMessageBox::Yes | QMessageBox::No , QMessageBox::No);
        if(answer == QMessageBox::Yes)
        {
            ui->CatalogsChairsTable->removeRow(ui->CatalogsChairsTable->currentRow());
        }
    }

}
