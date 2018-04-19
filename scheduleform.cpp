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

bool ScheduleForm::loadSchedule(QSqlDatabase* db_,QSqlQuery* query_)
{
    this->db = db_;
    this->query = query_;

    if(!query->exec("SELECT name FROM `Rooms`"))
    {
        qDebug()<<"Rooms query falure";
        loadResult = false;
        return false;
    }
    while(query->next())
    {
        ui->RoomsComboBox->addItem(query->value(0).toString());
    }
    ui->RoomsComboBox->model()->sort(0);
    ui->RoomsComboBox->setCurrentIndex(0);

    if(!query->exec("SELECT * FROM `Term`"))
    {
        qDebug()<<"Term query falure";
        loadResult = false;
        return false;
    }
    query->next();
    beginDate = QDate::fromString(query->value(0).toString(),"dd.MM.yyyy");
    endDate = QDate::fromString(query->value(1).toString(),"dd.MM.yyyy");
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
    if(!loadResult)
    {
        return false;
    }

    return loadResult;
}

void ScheduleForm::updateTable()
{
    if(ui->RoomsComboBox->count() > 0)
    {
        db->transaction();
        for(int i = 0; i < ui->ScheduleTable->rowCount(); i++)
        {
            for(int j = 0; j < ui->ScheduleTable->columnCount(); j++)
            {
                ui->ScheduleTable->item(i,j)->setBackground(Qt::white);
                ui->ScheduleTable->item(i,j)->setText("");
            }
        }
        ui->ScheduleTable->reset();
        ui->ScheduleTable->clearSpans();

        int roomId = -1;

        query->prepare("SELECT id FROM `Rooms` WHERE name=:roomName");
        query->bindValue(":roomName",ui->RoomsComboBox->currentText());
        if(!query->exec())
        {
            qDebug()<<"Room query falure";
            loadResult = false;
            return;
        }
        query->next();
        roomId = query->value(0).toInt();

        if(roomId != -1)
        {

            QDate curWeekBegin = beginDate.addDays(ui->WeekComboBox->currentIndex()*7);
            QDate curWeekEnd = beginDate.addDays(ui->WeekComboBox->currentIndex()*7).addDays(6);
            query->prepare("SELECT * FROM `Scheds` WHERE beginDate <= :curWeekBegin AND endDate >= :curWeekEnd AND roomId = :roomId");
            query->bindValue(":curWeekBegin",curWeekBegin.toString("yyyyMMdd"));
            query->bindValue(":curWeekEnd",curWeekEnd.toString("yyyyMMdd"));
            query->bindValue(":roomId",roomId);
            if(!query->exec())
            {
                qDebug()<<"Scheds query falure";
                loadResult = false;
                return;
            }
            while(query->next())
            {
                QString sumInfo;
                sumInfo.clear();
                QSqlQuery LoadsObj = QSqlQuery();
                LoadsObj.prepare("SELECT klassIdList FROM `Loads` WHERE id=:loadId");
                int loadId = query->value(4).toInt();
                LoadsObj.bindValue(":loadId",loadId);
                if(!LoadsObj.exec())
                {
                    qDebug()<<"LoadsObj query error!";
                    loadResult = false;
                    return;
                }
                LoadsObj.next();
                QStringList gr = LoadsObj.value(0).toString().split(" ");
                for(int i = 0; i < gr.size(); i++)
                {
                    if(gr[i] != "")
                    {
                        QSqlQuery GroupObj = QSqlQuery();
                        GroupObj.prepare("SELECT name FROM `Classes` WHERE id=:klassId");
                        GroupObj.bindValue(":klassId",gr[i]);
                        if(!GroupObj.exec())
                        {
                            qDebug()<<"Group query error!";
                            loadResult = false;
                            return;
                        }
                        GroupObj.next();
                        sumInfo += GroupObj.value(0).toString() + " ";
                    }
                }

                sumInfo = sumInfo.left(sumInfo.length()-2);
                sumInfo += "\n";


                QSqlQuery LoadGroupObj = QSqlQuery();
                LoadGroupObj.prepare("SELECT * FROM `LoadGroups` WHERE loadId=:loadId");
                LoadGroupObj.bindValue(":loadId",loadId);
                if(!LoadGroupObj.exec())
                {
                    qDebug()<<"LoadGroupObj query error!";
                    loadResult = false;
                    return;
                }
                LoadGroupObj.next();

                QSqlQuery TeacherObj = QSqlQuery();
                TeacherObj.prepare("SELECT * FROM `Teachers` WHERE id=:teacherId");
                TeacherObj.bindValue(":teacherId",LoadGroupObj.value(2).toInt());
                if(!TeacherObj.exec())
                {
                    qDebug()<<"TeacherObj query error!";
                    loadResult = false;
                    return;
                }
                TeacherObj.next();

                if(TeacherObj.value(1).toString()[0] != '_' && TeacherObj.value(1).toString()[0] != '=')
                    sumInfo += TeacherObj.value(1).toString() + " " + TeacherObj.value(2).toString()[0] + "." + TeacherObj.value(3).toString()[0] + ".";


                QSqlQuery SubjectObj = QSqlQuery();
                SubjectObj.prepare("SELECT * FROM `Subjects` WHERE id=:subjectId");
                SubjectObj.bindValue(":subjectId",LoadGroupObj.value(3));
                if(!SubjectObj.exec())
                {
                    qDebug()<<"SubjectObj query error!";
                    loadResult = false;
                    return;
                }
                SubjectObj.next();

                QString subject = SubjectObj.value(1).toString();
                sumInfo += subject + ". ";

                QSqlQuery StudyTypeObj = QSqlQuery();
                StudyTypeObj.prepare("SELECT * FROM `StudyTypes` WHERE id=:studyId");
                StudyTypeObj.bindValue(":studyId",LoadGroupObj.value(5));
                if(!StudyTypeObj.exec())
                {
                    qDebug()<<"StudyTypeObj query error!";
                    loadResult = false;
                    return;
                }

                StudyTypeObj.next();

                sumInfo += StudyTypeObj.value(1).toString();
                ui->ScheduleTable->item(query->value(1).toInt()-1,query->value(2).toInt()-1)->setText(sumInfo);
                ui->ScheduleTable->item(query->value(1).toInt()-1,query->value(2).toInt()-1)->setBackground(Qt::red);
            }


            for(int i = 0; i < ui->ScheduleTable->rowCount(); i++)
            {
                for(int j = 0; j < ui->ScheduleTable->columnCount(); j++)
                {
                    query->prepare("SELECT * FROM `Reserved` WHERE room=:room AND week=:week AND day=:day AND hour=:hour");
                    query->bindValue(":room",roomId);
                    query->bindValue(":week",ui->WeekComboBox->currentIndex());
                    query->bindValue(":day",i);
                    query->bindValue(":hour",j);
                    if(!query->exec())
                    {
                        qDebug()<<"Reserver query falure";
                        loadResult = false;
                        return;
                    }
                    int numberOfRows = 0;
                    if(query->last())
                    {
                        numberOfRows =  query->at() + 1;
                        query->first();
                        query->previous();
                    }
                    if(numberOfRows > 0)
                    {
                        query->next();
                        QString teacher = query->value(5).toString();
                        QString group = query->value(6).toString();
                        QString reason = query->value(7).toString();
                        if(!query->value(5).toString().isEmpty())
                        {
                            ui->ScheduleTable->item(i,j)->setText(teacher + "\n" + group + "\n" + reason);
                            ui->ScheduleTable->item(i,j)->setBackground(Qt::blue);
                        }
                    }

                }
            }
            for(int i = 0; i < ui->ScheduleTable->rowCount(); i++)
            {
                for(int j = 0; j < ui->ScheduleTable->columnCount(); j++)
                {
                    if(ui->ScheduleTable->item(i,j-1) != nullptr)
                        if(ui->ScheduleTable->item(i,j-1)->text() == ui->ScheduleTable->item(i,j)->text()
                                && !ui->ScheduleTable->item(i,j)->text().isEmpty() &&
                                ui->ScheduleTable->item(i,j)->backgroundColor() != Qt::blue)
                            ui->ScheduleTable->setSpan(i,j-1,1,2);
                }
            }
        }
        db->commit();

        query->finish();
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

void ScheduleForm::on_Multimedia_stateChanged(int arg1)
{

    if(loaded)
    {
        loaded = false;
        ui->RoomsComboBox->clear();

        if(arg1 == 0)
        {
            query->prepare("SELECT name FROM `Rooms` WHERE capacity >= :capacity");
            query->bindValue(":capacity",ui->capacity->value());
            if(!query->exec())
            {
                qDebug()<<"Rooms query falure";
            }
            while(query->next())
            {
                ui->RoomsComboBox->addItem(query->value(0).toString());
            }
        }
        else if(arg1 == 2)
        {
            query->prepare("SELECT name FROM `Rooms` WHERE capacity >= :capacity AND chairId IN (SELECT id FROM `Chairs` WHERE shortName == 'ММ')");
            query->bindValue(":capacity",ui->capacity->value());
            if(!query->exec())
            {
                qDebug()<<"Rooms query falure";
            }
            while(query->next())
            {
                ui->RoomsComboBox->addItem(query->value(0).toString());
            }
        }

        ui->RoomsComboBox->model()->sort(0);

        if(ui->RoomsComboBox->count() > 0)
            ui->RoomsComboBox->setCurrentIndex(0);
        else
            ui->RoomsComboBox->setCurrentIndex(-1);

        loaded = true;
        updateTable();
    }
}

void ScheduleForm::on_capacity_editingFinished()
{
    if(loaded)
    {
        loaded = false;
        ui->RoomsComboBox->clear();
        if(ui->Multimedia->isChecked())
        {
            query->prepare("SELECT name FROM `Rooms` WHERE capacity >= :capacity AND chairId IN (SELECT id FROM `Chairs` WHERE shortName == 'ММ')");
            query->bindValue(":capacity",ui->capacity->value());
            if(!query->exec())
            {
                qDebug()<<"Rooms query falure";
            }
            while(query->next())
            {
                ui->RoomsComboBox->addItem(query->value(0).toString());
            }
        }
        else
        {
            query->prepare("SELECT name FROM `Rooms` WHERE capacity >= :capacity");
            query->bindValue(":capacity",ui->capacity->value());
            if(!query->exec())
            {
                qDebug()<<"Rooms query falure";
            }
            while(query->next())
            {
                ui->RoomsComboBox->addItem(query->value(0).toString());
            }
        }
        ui->RoomsComboBox->model()->sort(0);
        if(ui->RoomsComboBox->count() > 0)
            ui->RoomsComboBox->setCurrentIndex(0);
        else
            ui->RoomsComboBox->setCurrentIndex(-1);

        loaded = true;
        updateTable();
    }


}


void ScheduleForm::on_ScheduleTable_cellDoubleClicked(int row, int column)
{
    if(ui->ScheduleTable->item(row,column)->text().isEmpty())
    {
        BookAudienceForm* baf = new BookAudienceForm();
        query->prepare("SELECT id FROM `Rooms` WHERE name=:roomName");
        query->bindValue(":roomName",ui->RoomsComboBox->currentText());
        if(!query->exec())
        {
            qDebug()<<"Room query falure";
        }
        query->next();
        baf->bookAudience(this,db,query,query->value(0).toInt(),ui->WeekComboBox->currentIndex(),row,column);
        baf->show();
    }

    if(ui->ScheduleTable->item(row,column)->backgroundColor() == Qt::blue)
    {
        BookAudienceForm* baf = new BookAudienceForm();
        query->prepare("SELECT id FROM `Rooms` WHERE name=:roomName");
        query->bindValue(":roomName",ui->RoomsComboBox->currentText());
        if(!query->exec())
        {
            qDebug()<<"Room query falure";
        }
        query->next();
        baf->editAudience(this,this->db,this->query,query->value(0).toInt(),ui->WeekComboBox->currentIndex(),row,column);
        baf->show();
    }
}
