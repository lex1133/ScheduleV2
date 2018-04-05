#include "sheduleform.h"
#include "ui_sheduleform.h"

SheduleForm::SheduleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SheduleForm)
{
    ui->setupUi(this);
}

SheduleForm::~SheduleForm()
{
    delete ui;
}

void SheduleForm::loadShedule(XMLParser *parser_)
{
    this->parser = parser_;
    for (QHash<int,Items::RoomObj>::iterator i = parser->getRooms()->begin(); i != parser->getRooms()->end(); ++i)
    {
        ui->RoomsComboBox->addItem(i.value().name);
    }

    QDate beginDate = QDate::fromString(parser->getTerm()->beginDate,"dd.MM.yyyy");
    QDate endDate = QDate::fromString(parser->getTerm()->endDate,"dd.MM.yyyy");
    int weeks = ceil(beginDate.daysTo(endDate)/7.0);
    updateTable();
}

void SheduleForm::updateTable()
{

}
