#include "scheduleform.h"
#include "ui_scheduleform.h"

ScheduleForm::ScheduleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScheduleForm)
{
    ui->setupUi(this);
}

ScheduleForm::~ScheduleForm()
{
    delete ui;
}

void ScheduleForm::loadSchedule(XMLParser *parser_)
{
    this->parser = parser_;
    for (QHash<int,Items::RoomObj>::iterator i = parser->getRooms()->begin(); i != parser->getRooms()->end(); ++i)
    {
        ui->RoomsComboBox->addItem(i.value().name);
    }
    ui->RoomsComboBox->model()->sort(0);
    ui->RoomsComboBox->setCurrentIndex(0);

    beginDate = QDate::fromString(parser->getTerm()->beginDate,"dd.MM.yyyy");
    endDate = QDate::fromString(parser->getTerm()->endDate,"dd.MM.yyyy");
    int weeks = ceil(beginDate.daysTo(endDate)/7.0);
    for(int i = 0; i < weeks; i++)
    {
        ui->WeekComboBox->addItem(beginDate.addDays(i*7).toString("dd.MM.yyyy") + " - " + beginDate.addDays(i*7).addDays(6).toString("dd.MM.yyyy") + " (" + QString::number(i+1) + ")" );
    }
    for(int i = 0; i < ui->ScheduleTable->rowCount(); i++)
    {
        for(int j = 0; j < ui->ScheduleTable->columnCount(); j++)
        {
            ui->ScheduleTable->setItem(i,j, new QTableWidgetItem());
        }
    }
    loaded = true;
    updateTable();

}

void ScheduleForm::updateTable()
{
    for(int i = 0; i < ui->ScheduleTable->rowCount(); i++)
    {
        for(int j = 0; j < ui->ScheduleTable->columnCount(); j++)
        {
            ui->ScheduleTable->item(i,j)->setBackground(Qt::white);
            ui->ScheduleTable->item(i,j)->setText("");
        }
    }
    ui->ScheduleTable->reset();

    int roomId = 0;

    for (QHash<int,Items::RoomObj>::iterator i = parser->getRooms()->begin(); i != parser->getRooms()->end(); ++i)
    {
        if(i.value().name == ui->RoomsComboBox->currentText())
        {
            roomId = i.key();
            break;
        }
    }

    for (QHash<int,Items::SchedObj>::iterator i = parser->getScheds()->begin(); i != parser->getScheds()->end(); ++i)
    {
        QDate curWeekBegin = beginDate.addDays(ui->WeekComboBox->currentIndex()*7);
        QDate curWeekEnd = beginDate.addDays(ui->WeekComboBox->currentIndex()*7).addDays(6);
        QDate tmp1 = QDate::fromString(i.value().beginDate,"dd.MM.yyyy");
        QDate tmp2 = QDate::fromString(i.value().endDate,"dd.MM.yyyy");
        if(i.value().roomId == roomId && tmp1 <= curWeekBegin && tmp2 >= curWeekEnd)
        {
            QString sumInfo;
            sumInfo.clear();
            Items::LoadObj load = parser->getLoads()->value(i.value().loadId);
            for(int i = 0; i < load.klassIdList.size(); i++)
            {
                sumInfo += parser->getClasses()->value(load.klassIdList[i]).name + ", ";
            }

            sumInfo = sumInfo.left(sumInfo.length()-2);
            sumInfo += "\n";
            Items::TeacherObj teacherObj = parser->getTeachers()->value(load.groups[0].teacherId);
            sumInfo += teacherObj.surname + " " + teacherObj.firstName + "." + teacherObj.secondName + ". \n";
            QString subject = parser->getSubjects()->value(load.groups[0].subjectId).fullName;
            sumInfo += subject;
            ui->ScheduleTable->item(i.value().day-1,i.value().hour-1)->setText(sumInfo);
            ui->ScheduleTable->item(i.value().day-1,i.value().hour-1)->setBackground(Qt::red);
        }
    }
    for(int i = 0; i < ui->ScheduleTable->rowCount(); i++)
    {
        for(int j = 0; j < ui->ScheduleTable->columnCount(); j++)
        {
            if(ui->ScheduleTable->item(i,j-1) != nullptr)
                if(ui->ScheduleTable->item(i,j-1)->text() == ui->ScheduleTable->item(i,j)->text() && !ui->ScheduleTable->item(i,j)->text().isEmpty())
                    ui->ScheduleTable->setSpan(i,j-1,1,2);
        }
    }
}


void ScheduleForm::on_RoomsComboBox_currentIndexChanged(int index)
{
    if(loaded)
        updateTable();
}

void ScheduleForm::on_WeekComboBox_currentIndexChanged(int index)
{
    if(loaded)
        updateTable();
}
