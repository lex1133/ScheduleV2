#include "catalogsform.h"
#include "ui_catalogsform.h"

CatalogsForm::CatalogsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CatalogsForm)
{
    ui->setupUi(this);
    ui->CatalogsTeachersTable->setColumnHidden(0,true);
    ui->CatalogsClassesTable->setColumnHidden(0,true);
    ui->CatalogsRoomsTable->setColumnHidden(0,true);
    ui->CatalogsChairsTable->setColumnHidden(0,true);

    QFileInfo fin(QApplication::arguments()[0]);
    sett = new QSettings(fin.absolutePath() + "\\settings.ini",QSettings::IniFormat);
    if(!sett->contains("PathToSave"))
    {
        QMessageBox::information(0,tr("Конфигурация не определнна"), tr("Файл конфигурации не найден или поврежден!<br>"
                                                                        "Пожалуйста произведите настройку программы."),QMessageBox::Ok);
        SettingsForm* sf = new SettingsForm(0);
        sf->show();
    }
}

CatalogsForm::~CatalogsForm()
{
    delete ui;
}

bool CatalogsForm::loadCatalogs(MainWindow* mw_, QSqlDatabase* db_,QSqlQuery* query_)
{
    this->db = db_;
    this->query = query_;
    this->mw = mw_;
    db->transaction();
    loadChairs();
    if(!loadResult)
    {
        return false;
    }
    loadTeachers();
    if(!loadResult)
    {
        return false;
    }
    loadRooms();
    if(!loadResult)
    {
        return false;
    }
    loadSubjects();
    if(!loadResult)
    {
        return false;
    }
    loadClasses();
    if(!loadResult)
    {
        return false;
    }
    db->commit();
    query->finish();
    return loadResult;
}

void CatalogsForm::closeCatalogs()
{
    ui->CatalogsSubtab->setCurrentIndex(0);
    ui->CatalogsChairsTable->setRowCount(0);
    ui->CatalogsRoomsTable->setRowCount(0);
    ui->CatalogsTeachersTable->setRowCount(0);
    ui->CatalogsSubjectsTable->setRowCount(0);
    ui->CatalogsClassesTable->setRowCount(0);
}

void CatalogsForm::loadChairs()
{
    if(!query->exec("SELECT * FROM `Chairs`"))
    {
        qDebug()<<"Chairs query error!";
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
    ui->CatalogsChairsTable->setRowCount(numberOfRows);
    while (query->next()) {
        ui->CatalogsChairsTable->setItem(query->value(0).toInt(),0, new QTableWidgetItem(query->value(0).toString()));
        ui->CatalogsChairsTable->setItem(query->value(0).toInt(),1, new QTableWidgetItem(query->value(1).toString()));
        ui->CatalogsChairsTable->setItem(query->value(0).toInt(),2, new QTableWidgetItem(query->value(2).toString()));
    }
}

void CatalogsForm::loadTeachers()
{
    if(!query->exec("SELECT * FROM `Teachers`"))
    {
        qDebug()<<"Teachers query error!";
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
    ui->CatalogsTeachersTable->setRowCount(numberOfRows);
    while (query->next()) {

        ui->CatalogsTeachersTable->setItem(query->value(0).toInt(),0, new QTableWidgetItem(query->value(0).toString()));
        ui->CatalogsTeachersTable->setItem(query->value(0).toInt(),1, new QTableWidgetItem(query->value(1).toString() + " " + query->value(2).toString()[0] + ". " + query->value(3).toString()[0] + "."));
        if(query->value(4).toInt() != -1)
        {
            QSqlQuery tmp = QSqlQuery();
            tmp.prepare("SELECT * FROM `Chairs` WHERE id=:chairId");
            tmp.bindValue(":chairId", query->value(4).toInt());
            if(!tmp.exec())
            {
                loadResult = false;
                return;
            }
            tmp.next();
            ui->CatalogsTeachersTable->setItem(query->value(0).toInt(),2, new QTableWidgetItem(tmp.value(1).toString()));
        }
        else
        {
            ui->CatalogsTeachersTable->setItem(query->value(0).toInt(),2, new QTableWidgetItem("Не определена"));
        }
    }

}

void CatalogsForm::loadRooms()
{

    if(!query->exec("SELECT * FROM `Rooms`"))
    {
        qDebug()<<"Rooms query error!";
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
    ui->CatalogsRoomsTable->setRowCount(numberOfRows);

    while (query->next())
    {
        ui->CatalogsRoomsTable->setItem(query->value(0).toInt(),0, new QTableWidgetItem(QString::number(query->value(0).toInt())));
        ui->CatalogsRoomsTable->setItem(query->value(0).toInt(),1, new QTableWidgetItem(query->value(1).toString()));
        ui->CatalogsRoomsTable->setItem(query->value(0).toInt(),2, new QTableWidgetItem(QString::number(query->value(2).toInt())));
        if(query->value(3).toInt() != -1)
        {
            QSqlQuery tmp = QSqlQuery();
            tmp.prepare("SELECT * FROM `Chairs` WHERE id=:chairId");
            tmp.bindValue(":chairId", query->value(4).toInt());
            if(!tmp.exec())
            {
                loadResult = false;
                return;
            }
            tmp.next();
            ui->CatalogsRoomsTable->setItem(query->value(0).toInt(),3, new QTableWidgetItem(tmp.value(1).toString()));
        }
        else
        {
            ui->CatalogsRoomsTable->setItem(query->value(0).toInt(),3, new QTableWidgetItem("Не определена"));
        }



    }
}

void CatalogsForm::loadSubjects()
{

    if(!query->exec("SELECT * FROM `Subjects`"))
    {
        qDebug()<<"Subjects query error!";
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
    ui->CatalogsSubjectsTable->setRowCount(numberOfRows);
    while (query->next())
    {
        ui->CatalogsSubjectsTable->setItem(query->value(0).toInt(),0, new QTableWidgetItem(query->value(2).toString()));
        ui->CatalogsSubjectsTable->setItem(query->value(0).toInt(),1, new QTableWidgetItem(query->value(1).toString()));

    }
}

void CatalogsForm::loadClasses()
{
    if(!query->exec("SELECT * FROM `Classes`"))
    {
        qDebug()<<"Classes query error!";
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
    ui->CatalogsClassesTable->setRowCount(numberOfRows);
    while (query->next())
    {
        ui->CatalogsClassesTable->setItem(query->value(0).toInt(),0, new QTableWidgetItem(QString::number(query->value(0).toInt())));
        ui->CatalogsClassesTable->setItem(query->value(0).toInt(),1, new QTableWidgetItem(query->value(1).toString()));
        ui->CatalogsClassesTable->setItem(query->value(0).toInt(),2, new QTableWidgetItem(QString::number(query->value(2).toInt())));
        ui->CatalogsClassesTable->setItem(query->value(0).toInt(),3, new QTableWidgetItem(QString::number(query->value(3).toInt())));
    }

}


void CatalogsForm::on_SubjectsSearchLine_textEdited(const QString &arg1)
{
    auto search = ui->CatalogsSubjectsTable->findItems(arg1,Qt::MatchStartsWith);
    for( int i = 0; i < ui->CatalogsSubjectsTable->rowCount(); ++i )
    {
        bool match = false;

        QTableWidgetItem *item = ui->CatalogsSubjectsTable->item( i, 0 );
        if( search.contains(item))
        {
            match = true;
        }

        ui->CatalogsSubjectsTable->setRowHidden( i, !match );
    }
}

void CatalogsForm::on_ClassesSearchLine_textEdited(const QString &arg1)
{
    auto search = ui->CatalogsClassesTable->findItems(arg1,Qt::MatchStartsWith);
    for( int i = 0; i < ui->CatalogsClassesTable->rowCount(); ++i )
    {
        bool match = false;

        QTableWidgetItem *item = ui->CatalogsClassesTable->item( i, 1 );
        if( search.contains(item))
        {
            match = true;
        }

        ui->CatalogsClassesTable->setRowHidden( i, !match );
    }
}

void CatalogsForm::on_RoomsSearchLine_textEdited(const QString &arg1)
{
    auto search = ui->CatalogsRoomsTable->findItems(arg1,Qt::MatchStartsWith);
    for( int i = 0; i < ui->CatalogsRoomsTable->rowCount(); ++i )
    {
        bool match = false;

        QTableWidgetItem *item = ui->CatalogsRoomsTable->item( i, 1 );
        if( search.contains(item))
        {
            match = true;
        }

        ui->CatalogsRoomsTable->setRowHidden( i, !match );
    }
}

void CatalogsForm::on_TeachersSearchLine_textEdited(const QString &arg1)
{
    auto search = ui->CatalogsTeachersTable->findItems(arg1,Qt::MatchContains);
    for( int i = 0; i < ui->CatalogsTeachersTable->rowCount(); ++i )
    {
        bool match = false;


        QTableWidgetItem *item = ui->CatalogsTeachersTable->item( i, 1 );
        if( search.contains(item))
        {
            match = true;
        }
        ui->CatalogsTeachersTable->setRowHidden( i, !match );

    }
}

void CatalogsForm::on_ChairsSearchLine_textEdited(const QString &arg1)
{
    auto search = ui->CatalogsChairsTable->findItems(arg1,Qt::MatchContains);
    int column = ui->ChairsSearchInCombo->currentIndex();
    for( int i = 0; i < ui->CatalogsChairsTable->rowCount(); ++i )
    {
        bool match = false;

        QTableWidgetItem *item = ui->CatalogsChairsTable->item( i, column );
        if( search.contains(item))
        {
            match = true;
        }

        ui->CatalogsChairsTable->setRowHidden( i, !match );
    }
}

void CatalogsForm::on_ChairsSearchInCombo_currentIndexChanged(int index)
{
    on_ChairsSearchLine_textEdited((ui->ChairsSearchLine->text()));
}

void CatalogsForm::drawSchedule(QPainter &painter,QRect pageRect, QString type, int row)
{
    float v = 15;
    int padding = 20;
    QList<int> verts;
    QList<int> hors;
    v = pageRect.width() / 1000.0;
    padding = v * 1.5;
    verts = QList<int>();
    hors = QList<int>();
    QPen pen;
    pen.setWidth(15);
    painter.setPen(pen);
    QVector<QString> hours;
    if(!query->exec("SELECT * FROM `Times`"))
    {
        qDebug()<<"Times query error";
    }
    query->next();
    while(query->next())
    {
        hours<<query->value(1).toString();
    }

    QVector<QString> daynamesFull {"Понедельник","Вторник","Среда","Четверг","Пятница","Суббота"};
    QFont f = QFont("Arial",v*0.65);

    verts.append(1 * v);
    verts.append(verts.last() + 15*v);
    for (int i = 0; i < 8; i++)
    {
        verts.append(verts.last() + 123*v);
    }

    hors.append(55 * v);
    hors.append(hors.last() + 15*v);
    for (int i = 0; i < 6; i++)
    {
        hors.append(hors.last() + 105*v);
    }


    for (int j = 0; j < hors.size(); j++)
    {
        painter.drawLine(verts.first(), hors[j], verts.last(), hors[j]);
    }

    painter.drawLine(verts[0], hors[0], verts[0], hors[hors.size() - 1]);
    painter.drawLine(verts[1], hors[0], verts[1], hors[hors.size() - 1]);
    painter.drawLine(verts[verts.size() - 1], hors[0], verts[verts.size() - 1], hors[hors.size() - 1]);


    for (int j = 1; j < verts.size() - 1; j++)
    {
        painter.drawLine(verts[j], hors[0], verts[j], hors[1]);
    }


    painter.setFont(f);

    for (int i = 0; i < hours.size(); i++)
    {
        int h = i + 1;
        painter.drawText(verts[h], hors[0],
                verts[h + 1] - verts[h], hors[1] - hors[0],
                Qt::AlignVCenter | Qt::AlignCenter,
                hours[i]);

    }

    for (int i = 0; i < daynamesFull.size(); i++)
    {
        int h = i + 2;
        drawRotatedText(painter, verts[0], hors[h], hors[h] - hors[h - 1], verts[1] - verts[0], daynamesFull[i]);
    }

    QFont ft = QFont("Arial",v*1.1);
    painter.setFont(ft);
    QString title;
    if(type == "class")
        title = ui->CatalogsClassesTable->item(row,1)->text();
    else if(type == "room")
        title = ui->CatalogsRoomsTable->item(row,1)->text();
    else if(type == "teacher")
        title = ui->CatalogsTeachersTable->item(row,1)->text();
    painter.drawText(verts[0], 20 * v,
            verts[verts.size() - 1] - verts[0] - 25*v, 40 * v - 25*v,
            Qt::AlignVCenter | Qt::AlignHCenter,
            title);
    if(type == "class")
        drawClassSched(painter, verts, hors, ui->CatalogsClassesTable->item(row,0)->text().toInt());
    if(type == "teacher")
        drawTeacherSched(painter, verts, hors, ui->CatalogsTeachersTable->item(row,0)->text().toInt());
    if(type == "room")
        drawRoomSched(painter, verts, hors, ui->CatalogsRoomsTable->item(row,0)->text().toInt());

}


void CatalogsForm::drawClassSched(QPainter &painter, QList<int> &verts, QList<int> &hors, int classId)
{
    db->transaction();
    query->prepare("SELECT * FROM `Classes` WHERE id=:classId");
    query->bindValue(":classId",classId);
    if(!query->exec())
    {
        qDebug()<<"Scheds query error!";
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


    QVector<QVector<QPair<int,QVector<SubInfo>>>> schedObj;
    schedObj.resize(6);
    for(int i = 0; i < 6; i++)
        schedObj[i].resize(8);
    query->prepare("SELECT * FROM `Scheds` WHERE loadId IN (select id from `Loads` where klassIdList like '% " + QString::number(classId) +" %')");
    if(!query->exec())
    {
        qDebug()<<"Scheds query error!";
        loadResult = false;
        return;
    }
    while (query->next())
    {
        QSqlQuery LoadObj = QSqlQuery();
        LoadObj.prepare("SELECT * FROM `Loads` WHERE id=:loadId");
        LoadObj.bindValue(":loadId",query->value(4));
        if(!LoadObj.exec())
        {
            qDebug()<<"Load query error!";
            loadResult = false;
            return;
        }
        LoadObj.next();

        SubInfo tmp;
        QDate beginDate = QDate::fromString(query->value(6).toString(),"yyyyMMdd");
        QDate endDate = QDate::fromString(query->value(7).toString(),"yyyyMMdd");

        QSqlQuery LoadGroupObj = QSqlQuery();
        LoadGroupObj.prepare("SELECT * FROM `LoadGroups` WHERE loadId=:loadId");
        int loadId = query->value(4).toInt();
        LoadGroupObj.bindValue(":loadId",loadId);
        if(!LoadGroupObj.exec())
        {
            qDebug()<<"LoadGroupObj query error!";
            loadResult = false;
            return;
        }
        numberOfRows = 0;
        if(LoadGroupObj.last())
        {
            numberOfRows =  LoadGroupObj.at() + 1;
            LoadGroupObj.first();
            LoadGroupObj.previous();
        }
        LoadGroupObj.next();

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
        tmp.name = subject;
        tmp.type = LoadGroupObj.value(5).toInt();
        tmp.groupsNum = numberOfRows;

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
            tmp.teacher = TeacherObj.value(1).toString() + " " + TeacherObj.value(2).toString()[0] + "." + TeacherObj.value(3).toString()[0] + ".";
        if(LoadGroupObj.value(4).toString().size() > 0)
        {

            QSqlQuery RoomObj = QSqlQuery();
            RoomObj.prepare("SELECT * FROM `Rooms` WHERE id=:roomId");
            RoomObj.bindValue(":roomId",LoadGroupObj.value(4).toInt());
            if(!RoomObj.exec())
            {
                qDebug()<<"RoomObj query error!";
                loadResult = false;
                return;
            }
            RoomObj.next();
            tmp.room = RoomObj.value(1).toString();
        }

        QPair<QDate, QDate> date;
        date.first = beginDate.addDays(query->value(1).toInt()-1);
        date.second = endDate.addDays(-(6-(query->value(1).toInt()-1)));
        tmp.dates.push_back(date);
        tmp.group = QString::number(query->value(3).toInt());
        bool f = false;
        for(int sub = 0; sub < schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second.size(); sub++)
        {
            auto tmp1 = schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second[sub];
            if(tmp1.equal(tmp))
            {
                schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second[sub].dates.push_back(tmp.dates[0]);
                std::sort(schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second[sub].dates.begin(),schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second[sub].dates.end(),
                        [](const QPair<QDate,QDate>& x,const QPair<QDate,QDate>& y) -> bool
                {
                    return x.first < y.first;
                }
                );
                f = true;
            }
        }
        if(!f)
            schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second.push_back(tmp);



    }


    drawSchedTable(painter,verts,hors,schedObj);


    db->commit();

}

void CatalogsForm::drawTeacherSched(QPainter &painter, QList<int> &verts, QList<int> &hors, int teacherId)
{
    db->transaction();
    QVector<QVector<QPair<int,QVector<SubInfo>>>> schedObj;
    schedObj.resize(6);
    for(int i = 0; i < 6; i++)
        schedObj[i].resize(8);
    query->prepare("SELECT * FROM `Scheds` WHERE loadId IN (select loadId from `LoadGroups` where teacherId = :teacherId)");
    query->bindValue(":teacherId", teacherId);
    if(!query->exec())
    {
        qDebug()<<"Scheds query error!";
        loadResult = false;
        return;
    }
    while (query->next())
    {
        QSqlQuery LoadObj = QSqlQuery();
        LoadObj.prepare("SELECT * FROM `Loads` WHERE id=:loadId");
        LoadObj.bindValue(":loadId",query->value(4));
        if(!LoadObj.exec())
        {
            qDebug()<<"Load query error!";
            loadResult = false;
            return;
        }
        LoadObj.next();

        SubInfo tmp;
        QDate beginDate = QDate::fromString(query->value(6).toString(),"yyyyMMdd");
        QDate endDate = QDate::fromString(query->value(7).toString(),"yyyyMMdd");

        QSqlQuery LoadGroupObj = QSqlQuery();
        LoadGroupObj.prepare("SELECT * FROM `LoadGroups` WHERE loadId=:loadId");
        int loadId = query->value(4).toInt();
        LoadGroupObj.bindValue(":loadId",loadId);
        if(!LoadGroupObj.exec())
        {
            qDebug()<<"LoadGroupObj query error!";
            loadResult = false;
            return;
        }
        int numberOfRows = 0;
        if(LoadGroupObj.last())
        {
            numberOfRows =  LoadGroupObj.at() + 1;
            LoadGroupObj.first();
            LoadGroupObj.previous();
        }
        LoadGroupObj.next();

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
        tmp.name = subject;
        tmp.type = LoadGroupObj.value(5).toInt();
        tmp.groupsNum = numberOfRows;

        QSqlQuery TeacherObj = QSqlQuery();
        TeacherObj.prepare("SELECT klassIdList FROM `Loads` WHERE id=:loadId");
        TeacherObj.bindValue(":loadId",query->value(4));
        if(!TeacherObj.exec())
        {
            qDebug()<<"Load query error!";
            loadResult = false;
            return;
        }
        TeacherObj.next();
        QStringList gr = TeacherObj.value(0).toString().split(" ");
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
                tmp.teacher += GroupObj.value(0).toString() + " ";
            }
        }
        if(LoadGroupObj.value(4).toString().size() > 0)
        {

            QSqlQuery RoomObj = QSqlQuery();
            RoomObj.prepare("SELECT * FROM `Rooms` WHERE id=:roomId");
            RoomObj.bindValue(":roomId",LoadGroupObj.value(4).toInt());
            if(!RoomObj.exec())
            {
                qDebug()<<"RoomObj query error!";
                loadResult = false;
                return;
            }
            RoomObj.next();
            tmp.room = RoomObj.value(1).toString();
        }

        QPair<QDate, QDate> date;
        date.first = beginDate.addDays(query->value(1).toInt()-1);
        date.second = endDate.addDays(-(6-(query->value(1).toInt()-1)));
        tmp.dates.push_back(date);
        tmp.group = QString::number(query->value(3).toInt());
        bool f = false;
        for(int sub = 0; sub < schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second.size(); sub++)
        {
            auto tmp1 = schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second[sub];
            if(tmp1.equal(tmp))
            {
                schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second[sub].dates.push_back(tmp.dates[0]);
                std::sort(schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second[sub].dates.begin(),schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second[sub].dates.end(),
                        [](const QPair<QDate,QDate>& x,const QPair<QDate,QDate>& y) -> bool
                {
                    return x.first < y.first;
                }
                );
                f = true;
            }
        }
        if(!f)
            schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second.push_back(tmp);



    }
    drawSchedTable(painter,verts,hors,schedObj);

    db->commit();
}

void CatalogsForm::drawRoomSched(QPainter &painter, QList<int> &verts, QList<int> &hors, int roomId)
{
    db->transaction();
    QVector<QVector<QPair<int,QVector<SubInfo>>>> schedObj;
    schedObj.resize(6);
    for(int i = 0; i < 6; i++)
        schedObj[i].resize(8);
    query->prepare("SELECT * FROM `Scheds` WHERE roomId=:roomId");
    query->bindValue(":roomId", roomId);
    if(!query->exec())
    {
        qDebug()<<"Scheds query error!";
        loadResult = false;
        return;
    }
    while (query->next())
    {
        QSqlQuery LoadObj = QSqlQuery();
        LoadObj.prepare("SELECT * FROM `Loads` WHERE id=:loadId");
        LoadObj.bindValue(":loadId",query->value(4));
        if(!LoadObj.exec())
        {
            qDebug()<<"Load query error!";
            loadResult = false;
            return;
        }
        LoadObj.next();

        SubInfo tmp;
        QDate beginDate = QDate::fromString(query->value(6).toString(),"yyyyMMdd");
        QDate endDate = QDate::fromString(query->value(7).toString(),"yyyyMMdd");

        QSqlQuery LoadGroupObj = QSqlQuery();
        LoadGroupObj.prepare("SELECT * FROM `LoadGroups` WHERE loadId=:loadId");
        int loadId = query->value(4).toInt();
        LoadGroupObj.bindValue(":loadId",loadId);
        if(!LoadGroupObj.exec())
        {
            qDebug()<<"LoadGroupObj query error!";
            loadResult = false;
            return;
        }
        int numberOfRows = 0;
        if(LoadGroupObj.last())
        {
            numberOfRows =  LoadGroupObj.at() + 1;
            LoadGroupObj.first();
            LoadGroupObj.previous();
        }
        LoadGroupObj.next();

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
        tmp.name = subject;
        tmp.type = LoadGroupObj.value(5).toInt();
        tmp.groupsNum = numberOfRows;

        QSqlQuery TeacherObj = QSqlQuery();
        TeacherObj.prepare("SELECT klassIdList FROM `Loads` WHERE id=:loadId");
        TeacherObj.bindValue(":loadId",query->value(4));
        if(!TeacherObj.exec())
        {
            qDebug()<<"Load query error!";
            loadResult = false;
            return;
        }
        TeacherObj.next();
        QStringList gr = TeacherObj.value(0).toString().split(" ");
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
                tmp.teacher += GroupObj.value(0).toString() + " ";
            }
        }
        if(LoadGroupObj.value(4).toString().size() > 0)
        {

            QSqlQuery RoomObj = QSqlQuery();
            RoomObj.prepare("SELECT * FROM `Rooms` WHERE id=:roomId");
            RoomObj.bindValue(":roomId",LoadGroupObj.value(4).toInt());
            if(!RoomObj.exec())
            {
                qDebug()<<"RoomObj query error!";
                loadResult = false;
                return;
            }
            RoomObj.next();
            tmp.room = RoomObj.value(1).toString();
        }

        QPair<QDate, QDate> date;
        date.first = beginDate.addDays(query->value(1).toInt()-1);
        date.second = endDate.addDays(-(6-(query->value(1).toInt()-1)));
        tmp.dates.push_back(date);
        tmp.group = QString::number(query->value(3).toInt());
        bool f = false;
        for(int sub = 0; sub < schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second.size(); sub++)
        {
            auto tmp1 = schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second[sub];
            if(tmp1.equal(tmp))
            {
                schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second[sub].dates.push_back(tmp.dates[0]);
                std::sort(schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second[sub].dates.begin(),schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second[sub].dates.end(),
                        [](const QPair<QDate,QDate>& x,const QPair<QDate,QDate>& y) -> bool
                {
                    return x.first < y.first;
                }
                );
                f = true;
            }
        }
        if(!f)
            schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second.push_back(tmp);



    }
    drawSchedTable(painter,verts,hors,schedObj);

    db->commit();
}

void CatalogsForm::drawSchedTable(QPainter &painter, QList<int> &verts, QList<int> &hors, QVector<QVector<QPair<int, QVector<SubInfo> > > > &schedObj)
{

    QFont f = QFont("Arial",14);;
    for(int i = 0; i < schedObj.size(); i++)
    {
        for(int j = 0; j < schedObj[i].size()-1; j++)
        {
            for(int k = 0; k < schedObj[i][j].second.size(); k++)
            {
                if(schedObj[i][j].second[k].type == 2)
                {
                    for(int p = 0; p < schedObj[i][j+1].second.size(); p++)
                    {
                        auto first = schedObj[i][j+1].second[p];
                        auto second = schedObj[i][j].second[k];
                        bool eq = first == second;
                        if(eq)
                        {
                            schedObj[i][j+1].second[p].pair = true;
                            schedObj[i][j].second[k].pair = true;
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < schedObj.size(); i++)
    {
        for(int j = 0; j < schedObj[i].size(); j++)
        {
            std::sort(schedObj[i][j].second.begin(),schedObj[i][j].second.end(),
                      [](const SubInfo& x,const SubInfo& y) -> bool
            {
                if(x.pair == false)
                    return false;
                if(x.type == 1 && y.type == 2)
                    return false;
                if(x.type == 2 && y.type == 1)
                    return true;
                if(x.type == 2 && y.type == 3)
                    return true;
                if(x.type == 3 && y.type == 2)
                    return false;
                return true;
            }
            );

            if(schedObj[i][j].second.size() > 0)
            {

                painter.drawLine(verts[1+j],hors[1+i]+schedObj[i][j].first+10,verts[1+j],hors[2+i]);
                if(schedObj[i][j].second[0].type == 2 && schedObj[i][j].second[0].pair == true)
                {
                    if(schedObj[i].size()-1 >= j+2)
                        if(schedObj[i][j+2].second.size() == 0)
                            if(verts.size() > 3+j)
                                painter.drawLine(verts[3+j],hors[1+i],verts[3+j],hors[2+i]);
                }
                else
                {
                    if(verts.size() > 2+j)
                        painter.drawLine(verts[2+j],hors[1+i],verts[2+j],hors[2+i]);
                }
            }

            painter.setFont(f);

            if(schedObj[i][j].second.isEmpty() && schedObj[i][j].first == 0)
            {
                painter.drawLine(verts[1+j],hors[1+i]+10,verts[1+j],hors[2+i]+10);
            }
            int height = hors[2] - hors[1];
            int daySize = 0;
            if(schedObj[i][j].second.size() > 0)
                if(schedObj[i][j].second[0].type == 2 && schedObj[i][j].second[0].pair)
                {
                    if(schedObj[i][j+1].second.size() > schedObj[i][j].second.size())
                    {
                        daySize = schedObj[i][j+1].second.size();
                    }
                    else
                    {
                        daySize = schedObj[i][j].second.size();
                    }
                }
                else
                {
                    daySize = schedObj[i][j].second.size();
                }
            for(int k = 0; k < schedObj[i][j].second.size(); k++)
            {
                if(!schedObj[i][j].second[k].name.isEmpty())
                {

                    bool datesNorm = false;
                    while(!datesNorm)
                    {
                        datesNorm = true;
                        for(int date = 0; date < schedObj[i][j].second[k].dates.size()-1; date++)
                        {
                            if(schedObj[i][j].second[k].dates[date].second.daysTo(schedObj[i][j].second[k].dates[date+1].first) == 7)
                            {
                                datesNorm = false;
                                schedObj[i][j].second[k].dates[date].second = schedObj[i][j].second[k].dates[date+1].second;
                                schedObj[i][j].second[k].dates.removeAt(date+1);
                            }

                        }
                    }
                    if(k == 0 && schedObj[i][j].first != 0)
                    {
                        painter.drawLine(verts[2+j],hors[1+i],verts[2+j],hors[1+i]+schedObj[i][j].first+10);
                        painter.drawLine(verts[2+j],hors[1+i]+schedObj[i][j].first+10,verts[j],hors[1+i]+schedObj[i][j].first+10);
                        if(schedObj[i][j].second[k].type == 2 && schedObj[i][j].second[k].pair == true)
                            painter.drawLine(verts[2+j],hors[1+i]+schedObj[i][j].first+10,verts[j+3],hors[1+i]+schedObj[i][j].first+10);
                    }
                    if(k>0)
                        if(schedObj[i][j].first != 0 && !schedObj[i][j].second[k].pair &&  schedObj[i][j].second[k-1].type == 2)
                        {
                            if(schedObj[i][j].second[k-1].pair == true)
                                painter.drawLine(verts[1+j],hors[1+i]+schedObj[i][j].first+10,verts[j+3],hors[1+i]+schedObj[i][j].first+10);
                            else
                                painter.drawLine(verts[1+j],hors[1+i]+schedObj[i][j].first+10,verts[j+2],hors[1+i]+schedObj[i][j].first+10);

                            painter.drawLine(verts[2+j],hors[1+i]+schedObj[i][j].first+10,verts[2+j],hors[2+i]);
                        }
                    QString curSub;
                    curSub.clear();

                    QSqlQuery StudyTypeObj = QSqlQuery();
                    StudyTypeObj.prepare("SELECT * FROM `StudyTypes` WHERE id=:studyId");
                    StudyTypeObj.bindValue(":studyId",schedObj[i][j].second[k].type);
                    if(!StudyTypeObj.exec())
                    {
                        qDebug()<<"StudyTypeObj query error!";
                        loadResult = false;
                        return;
                    }

                    StudyTypeObj.next();

                    curSub += schedObj[i][j].second[k].name + ". " + schedObj[i][j].second[k].teacher + " " + StudyTypeObj.value(1).toString()
                            + (schedObj[i][j].second[k].type == 2 ?(schedObj[i][j].second[k].groupsNum > 1 ? (schedObj[i][j].second[k].group == QString::number(1) ? " (Б) " : " (А) ") : ""): "") + ". " + schedObj[i][j].second[k].room + ". " + "[";
                    for(int date = 0; date < schedObj[i][j].second[k].dates.size(); date++)
                    {
                        if(schedObj[i][j].second[k].dates[date].first.toString("dd.MM") != schedObj[i][j].second[k].dates[date].second.toString("dd.MM"))
                        {
                            if(schedObj[i][j].second[k].dates[date].first.daysTo(schedObj[i][j].second[k].dates[date].second) > 7)
                            {
                                curSub += schedObj[i][j].second[k].dates[date].first.toString("dd.MM") + " - " + schedObj[i][j].second[k].dates[date].second.toString("dd.MM") + " к.н.";
                            }
                            else
                            {
                                curSub += schedObj[i][j].second[k].dates[date].first.toString("dd.MM") + ", " + schedObj[i][j].second[k].dates[date].second.toString("dd.MM");
                            }


                        }
                        else
                        {
                            curSub += schedObj[i][j].second[k].dates[date].first.toString("dd.MM");
                        }
                        if(date != schedObj[i][j].second[k].dates.size() -1)
                            curSub += ", ";
                    }
                    curSub += "]";
                    if(curSub != ". . . []")
                    {

                        QRect br;
                        QRect drawArea;
                        bool complex = false;
                        if(schedObj[i].size() > j+1)
                        {
                            for(int p = 0; p < schedObj[i][j+1].second.size(); p++)
                            {
                                auto first = schedObj[i][j+1].second[p];
                                auto second = schedObj[i][j].second[k];
                                bool eq = first == second;
                                if(eq && schedObj[i][j].second[k].type == 2)
                                {
                                    complex = true;
                                    schedObj[i][j+1].second.removeAt(p);

                                    drawArea = QRect(verts[1+j]+10,hors[1+i]+10+schedObj[i][j].first,verts[3]-verts[1]-20,((height-schedObj[i][j].first)/(daySize-k))-5);
                                    break;
                                }
                            }
                        }
                        if(!complex)
                        {
                            drawArea = QRect(verts[1+j]+10,hors[1+i]+10+schedObj[i][j].first,verts[2]-verts[1]-20,((height-schedObj[i][j].first)/(daySize-k))-5);
                        }
                        fillRect(painter,curSub,drawArea,Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap,f);
                        painter.drawText(drawArea,Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap,curSub,&br);

                        schedObj[i][j].first += br.height();

                        if(schedObj[i].size()-1 >= j+1 && complex)
                            schedObj[i][j+1].first = schedObj[i][j].first;

                    }
                }

            }




        }
    }
}

void CatalogsForm::drawRotatedText(QPainter &painter, int x, int y, int width, int height, const QString &text)
{
    painter.save();
    painter.translate(x, y);
    painter.rotate(270);
    painter.drawText(0, 0, width, height, Qt::AlignVCenter | Qt::AlignCenter, text);
    painter.restore();
}

void CatalogsForm::fillRect(QPainter& painter,QString str,QRect rect, int flags, QFont font)
{
    painter.setFont(font);
    QRect br;
    br = painter.fontMetrics().boundingRect(rect,flags,str);
    while(br.height() > rect.height() || br.width() > rect.width())
    {
        font.setPointSizeF(font.pointSizeF()*0.95);
        painter.setFont(font);
        br = painter.fontMetrics().boundingRect(rect,flags,str);
    }
}

void CatalogsForm::on_CatalogsChairsTable_cellDoubleClicked(int row, int column)
{

}

void CatalogsForm::on_CatalogsClassesTable_cellDoubleClicked(int row, int column)
{
    QTime timer;
    timer.start();
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setColorMode(QPrinter::GrayScale);
    printer.setOrientation(QPrinter::Landscape);
    printer.setPageSize(QPrinter::A4);
    QDir d;
    if(!d.exists(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Группы\\"))
        d.mkpath(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Группы\\");
    printer.setOutputFileName(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Группы\\" + ui->CatalogsClassesTable->item(row,1)->text() + ".pdf");
    QPainter painter;
    if(!painter.begin(&printer))
    {
        qWarning("Falied");
    }

    else
    {

        drawSchedule(painter, printer.pageRect(),"class",row);

        painter.end();
        QMessageBox::information(this,tr("Информация"),tr("Экспорт завершен!"),QMessageBox::Ok);
        qDebug()<<"Time elapsed: "<<timer.elapsed();
    }
}


void CatalogsForm::on_ExportAllClassesButton_clicked()
{

    QProgressDialog progress("Сохранение расписаний","Стоп", 0, ui->CatalogsClassesTable->rowCount(), this);
    progress.setWindowModality(Qt::WindowModal);
    for(int i = 0; i < ui->CatalogsClassesTable->rowCount(); i++)
    {
        if (progress.wasCanceled())
            break;
        progress.setValue(i);
        progress.setLabelText("Сохранение расписаний " + QString::number(i+1) + "/" + QString::number(ui->CatalogsClassesTable->rowCount()));
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setColorMode(QPrinter::GrayScale);
        printer.setOrientation(QPrinter::Landscape);
        printer.setPageSize(QPrinter::A4);
        QDir d;
        if(!d.exists(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Группы\\"))
            d.mkpath(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Группы\\");
        printer.setOutputFileName(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Группы\\" + ui->CatalogsClassesTable->item(i,1)->text() + ".pdf");
        QPainter painter;
        if(!painter.begin(&printer))
        {
            qWarning("Falied");
        }

        else
        {
            drawSchedule(painter, printer.pageRect(),"class",i);

            painter.end();
        }
    }

    progress.setValue(ui->CatalogsClassesTable->rowCount());

    QMessageBox::information(this,tr("Информация"),tr("Экспорт завершен!"),QMessageBox::Ok);
}

void CatalogsForm::on_CatalogsTeachersTable_cellDoubleClicked(int row, int column)
{
    QTime timer;
    timer.start();
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setColorMode(QPrinter::GrayScale);
    printer.setOrientation(QPrinter::Landscape);
    printer.setPageSize(QPrinter::A4);
    QDir d;
    if(!d.exists(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Преподаватели\\"))
        d.mkpath(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Преподаватели\\");
    printer.setOutputFileName(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Преподаватели\\" + ui->CatalogsTeachersTable->item(row,1)->text() + ".pdf");
    QPainter painter;
    if(!painter.begin(&printer))
    {
        qWarning("Falied");
    }

    else
    {

        drawSchedule(painter, printer.pageRect(),"teacher",row);

        painter.end();
    }


    qDebug()<<"Time elapsed: "<<timer.elapsed();

    QMessageBox::information(this,tr("Информация"),tr("Экспорт завершен!"),QMessageBox::Ok);
}

void CatalogsForm::on_ExportAllTeachersButton_clicked()
{
    QProgressDialog progress("Сохранение расписаний","Стоп", 0, ui->CatalogsTeachersTable->rowCount(), this);
    progress.setWindowModality(Qt::WindowModal);
    for(int i = 0; i < ui->CatalogsTeachersTable->rowCount(); i++)
    {
        if (progress.wasCanceled())
            break;
        progress.setValue(i);
        progress.setLabelText("Сохранение расписаний " + QString::number(i+1) + "/" + QString::number(ui->CatalogsTeachersTable->rowCount()));
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setColorMode(QPrinter::GrayScale);
        printer.setOrientation(QPrinter::Landscape);
        printer.setPageSize(QPrinter::A4);
        QDir d;
        if(!d.exists(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Преподаватели\\"))
            d.mkpath(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Преподаватели\\");
        printer.setOutputFileName(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Преподаватели\\" + ui->CatalogsTeachersTable->item(i,1)->text() + ".pdf");
        QPainter painter;
        if(!painter.begin(&printer))
        {
            qWarning("Falied");
        }

        else
        {
            drawSchedule(painter, printer.pageRect(),"teacher",i);

            painter.end();
        }
    }

    progress.setValue(ui->CatalogsTeachersTable->rowCount());

    QMessageBox::information(this,tr("Информация"),tr("Экспорт завершен!"),QMessageBox::Ok);
}

void CatalogsForm::on_ExportSelectedClassesButton_clicked()
{
    auto selectedClasses = ui->CatalogsClassesTable->selectedItems();
    QProgressDialog progress("Сохранение расписаний","Стоп", 0, selectedClasses.count()/3, this);
    progress.setWindowModality(Qt::WindowModal);
    for(int i = 0; i < selectedClasses.count(); i+=3)
    {
        if (progress.wasCanceled())
            break;
        progress.setValue(i/3);
        progress.setLabelText("Сохранение расписаний " + QString::number(i/3+1) + "/" + QString::number(selectedClasses.count()/3));
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setColorMode(QPrinter::GrayScale);
        printer.setOrientation(QPrinter::Landscape);
        printer.setPageSize(QPrinter::A4);
        QDir d;
        if(!d.exists(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Группы\\"))
            d.mkpath(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Группы\\");
        printer.setOutputFileName(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Группы\\" + ui->CatalogsClassesTable->item(selectedClasses[i]->row(),1)->text() + ".pdf");
        QPainter painter;
        if(!painter.begin(&printer))
        {
            qWarning("Falied");
        }

        else
        {

            drawSchedule(painter, printer.pageRect(),"class",selectedClasses[i]->row());

            painter.end();
        }
    }

    progress.setValue(selectedClasses.count()/3);

    QMessageBox::information(this,tr("Информация"),tr("Экспорт завершен!"),QMessageBox::Ok);
}

void CatalogsForm::on_CatalogsRoomsTable_cellDoubleClicked(int row, int column)
{
    QTime timer;
    timer.start();
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setColorMode(QPrinter::GrayScale);
    printer.setOrientation(QPrinter::Landscape);
    printer.setPageSize(QPrinter::A4);
    QDir d;
    if(!d.exists(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Аудитории\\"))
        d.mkpath(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Аудитории\\");
    QString str = ui->CatalogsRoomsTable->item(row,1)->text();
    while(str.contains('/'))
        str.replace(str.indexOf('/'),1,'-');
    printer.setOutputFileName(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Аудитории\\" + str + ".pdf");
    QPainter painter;
    if(!painter.begin(&printer))
    {
        qWarning("Falied");
    }
    else
    {

        drawSchedule(painter, printer.pageRect(),"room",row);

        painter.end();
    }

    qDebug()<<"Time elapsed: "<<timer.elapsed();

    QMessageBox::information(this,tr("Информация"),tr("Экспорт завершен!"),QMessageBox::Ok);
}

void CatalogsForm::on_ExportAllRoomsButton_clicked()
{
    QProgressDialog progress("Сохранение расписаний","Стоп", 0, ui->CatalogsRoomsTable->rowCount(), this);
    progress.setWindowModality(Qt::WindowModal);
    for(int i = 0; i < ui->CatalogsRoomsTable->rowCount(); i++)
    {
        if (progress.wasCanceled())
            break;
        progress.setValue(i);
        progress.setLabelText("Сохранение расписаний " + QString::number(i+1) + "/" + QString::number(ui->CatalogsRoomsTable->rowCount()));
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setColorMode(QPrinter::GrayScale);
        printer.setOrientation(QPrinter::Landscape);
        printer.setPageSize(QPrinter::A4);
        QDir d;
        if(!d.exists(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Аудитории\\"))
            d.mkpath(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Аудитории\\");
        QString str = ui->CatalogsRoomsTable->item(i,1)->text();
        while(str.contains('/'))
            str.replace(str.indexOf('/'),1,'-');
        printer.setOutputFileName(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Аудитории\\" + str + ".pdf");
        QPainter painter;
        if(!painter.begin(&printer))
        {
            qWarning("Falied");
        }
        else
        {

            drawSchedule(painter, printer.pageRect(),"room",i);

            painter.end();
        }
    }

    progress.setValue(ui->CatalogsRoomsTable->rowCount());

    QMessageBox::information(this,tr("Информация"),tr("Экспорт завершен!"),QMessageBox::Ok);
}

void CatalogsForm::on_ExportSelectedRoomsPutton_clicked()
{
    auto selectedRooms = ui->CatalogsRoomsTable->selectedItems();
    QProgressDialog progress("Сохранение расписаний","Стоп", 0, selectedRooms.count()/3, this);
    progress.setWindowModality(Qt::WindowModal);
    for(int i = 0; i < selectedRooms.count(); i+=3)
    {
        if (progress.wasCanceled())
            break;
        progress.setValue(i/3);
        progress.setLabelText("Сохранение расписаний " + QString::number(i/3+1) + "/" + QString::number(selectedRooms.count()/3));
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setColorMode(QPrinter::GrayScale);
        printer.setOrientation(QPrinter::Landscape);
        printer.setPageSize(QPrinter::A4);
        QDir d;
        if(!d.exists(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Аудитории\\"))
            d.mkpath(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Аудитории\\");
        QString str = ui->CatalogsRoomsTable->item(i,1)->text();
        while(str.contains('/'))
            str.replace(str.indexOf('/'),1,'-');
        printer.setOutputFileName(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Аудитории\\" + str + ".pdf");
        QPainter painter;
        if(!painter.begin(&printer))
        {
            qWarning("Falied");
        }

        else
        {
            drawSchedule(painter, printer.pageRect(),"room",selectedRooms[i]->row());

            painter.end();
        }
    }

    progress.setValue(selectedRooms.count()/3);

    QMessageBox::information(this,tr("Информация"),tr("Экспорт завершен!"),QMessageBox::Ok);
}

void CatalogsForm::on_ExportSelectedTeachersButton_clicked()
{
    auto selectedTeachers = ui->CatalogsTeachersTable->selectedItems();
    QProgressDialog progress("Сохранение расписаний","Стоп", 0, selectedTeachers.count()/2, this);
    progress.setWindowModality(Qt::WindowModal);
    for(int i = 0; i < selectedTeachers.count(); i+=2)
    {
        if (progress.wasCanceled())
            break;
        progress.setValue(i/3);
        progress.setLabelText("Сохранение расписаний " + QString::number(i/2+1) + "/" + QString::number(selectedTeachers.count()/2));
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setColorMode(QPrinter::GrayScale);
        printer.setOrientation(QPrinter::Landscape);
        printer.setPageSize(QPrinter::A4);
        QDir d;
        if(!d.exists(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Преподаватели\\"))
            d.mkpath(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Преподаватели\\");
        printer.setOutputFileName(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Преподаватели\\" + ui->CatalogsTeachersTable->item(selectedTeachers[i]->row(),1)->text() + ".pdf");
        QPainter painter;
        if(!painter.begin(&printer))
        {
            qWarning("Falied");
        }

        else
        {
            drawSchedule(painter, printer.pageRect(),"teacher",selectedTeachers[i]->row());

            painter.end();
        }
    }

    progress.setValue(selectedTeachers.count()/2);

    QMessageBox::information(this,tr("Информация"),tr("Экспорт завершен!"),QMessageBox::Ok);
}

void CatalogsForm::on_ExportTeacherToGoogle_clicked()
{

    auto selectedTeachers = ui->CatalogsTeachersTable->selectedItems();
    db->transaction();
    QVector<QVector<QPair<int,QVector<SubInfo>>>> schedObj;
    schedObj.resize(6);
    for(int i = 0; i < 6; i++)
        schedObj[i].resize(8);
    query->prepare("SELECT * FROM `Scheds` WHERE loadId IN (select loadId from `LoadGroups` where teacherId = :teacherId)");
    query->bindValue(":teacherId", ui->CatalogsTeachersTable->item(ui->CatalogsTeachersTable->currentRow(),0)->text());
    if(!query->exec())
    {
        qDebug()<<"Scheds query error!";
        loadResult = false;
        return;
    }
    while (query->next())
    {
        QSqlQuery LoadObj = QSqlQuery();
        LoadObj.prepare("SELECT * FROM `Loads` WHERE id=:loadId");
        LoadObj.bindValue(":loadId",query->value(4));
        if(!LoadObj.exec())
        {
            qDebug()<<"Load query error!";
            loadResult = false;
            return;
        }
        LoadObj.next();

        SubInfo tmp;
        QDate beginDate = QDate::fromString(query->value(6).toString(),"yyyyMMdd");
        QDate endDate = QDate::fromString(query->value(7).toString(),"yyyyMMdd");

        QSqlQuery LoadGroupObj = QSqlQuery();
        LoadGroupObj.prepare("SELECT * FROM `LoadGroups` WHERE loadId=:loadId");
        int loadId = query->value(4).toInt();
        LoadGroupObj.bindValue(":loadId",loadId);
        if(!LoadGroupObj.exec())
        {
            qDebug()<<"LoadGroupObj query error!";
            loadResult = false;
            return;
        }
        int numberOfRows = 0;
        if(LoadGroupObj.last())
        {
            numberOfRows =  LoadGroupObj.at() + 1;
            LoadGroupObj.first();
            LoadGroupObj.previous();
        }
        LoadGroupObj.next();

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
        tmp.name = subject;
        tmp.type = LoadGroupObj.value(5).toInt();
        tmp.groupsNum = numberOfRows;

        QSqlQuery TeacherObj = QSqlQuery();
        TeacherObj.prepare("SELECT klassIdList FROM `Loads` WHERE id=:loadId");
        TeacherObj.bindValue(":loadId",query->value(4));
        if(!TeacherObj.exec())
        {
            qDebug()<<"Load query error!";
            loadResult = false;
            return;
        }
        TeacherObj.next();
        QStringList gr = TeacherObj.value(0).toString().split(" ");
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
                tmp.teacher += GroupObj.value(0).toString() + " ";
            }
        }
        if(LoadGroupObj.value(4).toString().size() > 0)
        {

            QSqlQuery RoomObj = QSqlQuery();
            RoomObj.prepare("SELECT * FROM `Rooms` WHERE id=:roomId");
            RoomObj.bindValue(":roomId",LoadGroupObj.value(4).toInt());
            if(!RoomObj.exec())
            {
                qDebug()<<"RoomObj query error!";
                loadResult = false;
                return;
            }
            RoomObj.next();
            tmp.room = RoomObj.value(1).toString();
        }

        QPair<QDate, QDate> date;
        date.first = beginDate.addDays(query->value(1).toInt()-1);
        date.second = endDate.addDays(-(6-(query->value(1).toInt()-1)));
        tmp.dates.push_back(date);
        tmp.group = QString::number(query->value(3).toInt());
        schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second.push_back(tmp);
    }

    db->commit();
    QDir d;
    if(!d.exists(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Преподаватели\\Google\\"))
        d.mkpath(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Преподаватели\\Google\\");
    QFile outputFile(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Преподаватели\\Google\\" + ui->CatalogsTeachersTable->item(ui->CatalogsTeachersTable->currentRow(),1)->text() + ".csv");
    outputFile.open(QIODevice::WriteOnly);
    if(!outputFile.isOpen()){
        qDebug() <<"- Error, unable to open for output";
    }
    QTextStream outStream(&outputFile);

      outStream << "Subject,Start Date,Start Time,End Date,End Time,Description,Location\n";

      QVector<QString> hours;
      if(!query->exec("SELECT * FROM `Times`"))
      {
          qDebug()<<"Times query error";
      }
      query->next();
      while(query->next())
      {
          hours<<query->value(1).toString();
      }
    for(int i = 0; i < schedObj.size(); i++)
    {
        for(int j = 0; j < schedObj[i].size(); j++)
        {
            for(int k = 0; k < schedObj[i][j].second.size(); k++)
            {
                outStream<<"\""<<schedObj[i][j].second[k].name<<"\""<<","<<schedObj[i][j].second[k].dates[0].first.toString("dd/MM/yy")<<","<<hours[j].mid(0,5)<<","<<
                           schedObj[i][j].second[k].dates[0].second.toString("dd/MM/yy")<<","<<hours[j].mid(8,5)<<","<<"\""<<schedObj[i][j].second[k].teacher<<"\""<<","<<schedObj[i][j].second[k].room<<"\n";
            }
        }
    }

    outputFile.close();

    QMessageBox::information(this,tr("Информация"),tr("Экспорт завершен!"),QMessageBox::Ok);
}


void CatalogsForm::on_ExportClassToGoogle_clicked()
{
    db->transaction();
    query->prepare("SELECT * FROM `Classes` WHERE id=:classId");
    query->bindValue(":classId",ui->CatalogsClassesTable->item(ui->CatalogsClassesTable->currentRow(),0)->text());
    if(!query->exec())
    {
        qDebug()<<"Scheds query error!";
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


    QVector<QVector<QPair<int,QVector<SubInfo>>>> schedObj;
    schedObj.resize(6);
    for(int i = 0; i < 6; i++)
        schedObj[i].resize(8);
    query->prepare("SELECT * FROM `Scheds` WHERE loadId IN (select id from `Loads` where klassIdList like '% " + ui->CatalogsClassesTable->item(ui->CatalogsClassesTable->currentRow(),0)->text() +" %')");
    if(!query->exec())
    {
        qDebug()<<"Scheds query error!";
        loadResult = false;
        return;
    }
    while (query->next())
    {
        QSqlQuery LoadObj = QSqlQuery();
        LoadObj.prepare("SELECT * FROM `Loads` WHERE id=:loadId");
        LoadObj.bindValue(":loadId",query->value(4));
        if(!LoadObj.exec())
        {
            qDebug()<<"Load query error!";
            loadResult = false;
            return;
        }
        LoadObj.next();

        SubInfo tmp;
        QDate beginDate = QDate::fromString(query->value(6).toString(),"yyyyMMdd");
        QDate endDate = QDate::fromString(query->value(7).toString(),"yyyyMMdd");

        QSqlQuery LoadGroupObj = QSqlQuery();
        LoadGroupObj.prepare("SELECT * FROM `LoadGroups` WHERE loadId=:loadId");
        int loadId = query->value(4).toInt();
        LoadGroupObj.bindValue(":loadId",loadId);
        if(!LoadGroupObj.exec())
        {
            qDebug()<<"LoadGroupObj query error!";
            loadResult = false;
            return;
        }
        numberOfRows = 0;
        if(LoadGroupObj.last())
        {
            numberOfRows =  LoadGroupObj.at() + 1;
            LoadGroupObj.first();
            LoadGroupObj.previous();
        }
        LoadGroupObj.next();

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
        tmp.name = subject;
        tmp.type = LoadGroupObj.value(5).toInt();
        tmp.groupsNum = numberOfRows;

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
            tmp.teacher = TeacherObj.value(1).toString() + " " + TeacherObj.value(2).toString()[0] + "." + TeacherObj.value(3).toString()[0] + ".";
        if(LoadGroupObj.value(4).toString().size() > 0)
        {

            QSqlQuery RoomObj = QSqlQuery();
            RoomObj.prepare("SELECT * FROM `Rooms` WHERE id=:roomId");
            RoomObj.bindValue(":roomId",LoadGroupObj.value(4).toInt());
            if(!RoomObj.exec())
            {
                qDebug()<<"RoomObj query error!";
                loadResult = false;
                return;
            }
            RoomObj.next();
            tmp.room = RoomObj.value(1).toString();
        }

        QPair<QDate, QDate> date;
        date.first = beginDate.addDays(query->value(1).toInt()-1);
        date.second = endDate.addDays(-(6-(query->value(1).toInt()-1)));
        tmp.dates.push_back(date);
        tmp.group = QString::number(query->value(3).toInt());
        schedObj[query->value(1).toInt()-1][query->value(2).toInt()-1].second.push_back(tmp);

    }

    db->commit();
    QDir d;
    if(!d.exists(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Группы\\Google\\"))
        d.mkpath(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Группы\\Google\\");
    QFile outputFile(sett->value("PathToSave").toString() + "\\Расписание для печати\\" + mw->curProject + "\\Группы\\Google\\" + ui->CatalogsClassesTable->item(ui->CatalogsClassesTable->currentRow(),1)->text() + ".csv");
    outputFile.open(QIODevice::WriteOnly);
    if(!outputFile.isOpen()){
        qDebug() <<"- Error, unable to open for output";
    }
    QTextStream outStream(&outputFile);

      outStream << "Subject,Start Date,Start Time,End Date,End Time,Description,Location\n";

      QVector<QString> hours;
      if(!query->exec("SELECT * FROM `Times`"))
      {
          qDebug()<<"Times query error";
      }
      query->next();
      while(query->next())
      {
          hours<<query->value(1).toString();
      }
    for(int i = 0; i < schedObj.size(); i++)
    {
        for(int j = 0; j < schedObj[i].size(); j++)
        {
            for(int k = 0; k < schedObj[i][j].second.size(); k++)
            {
                outStream<<"\""<<schedObj[i][j].second[k].name<<"\""<<","<<schedObj[i][j].second[k].dates[0].first.toString("dd/MM/yy")<<","<<hours[j].mid(0,5)<<","<<
                           schedObj[i][j].second[k].dates[0].second.toString("dd/MM/yy")<<","<<hours[j].mid(8,5)<<","<<"\""<<schedObj[i][j].second[k].teacher<<"\""<<","<<schedObj[i][j].second[k].room<<"\n";
            }
        }
    }

    outputFile.close();

    QMessageBox::information(this,tr("Информация"),tr("Экспорт завершен!"),QMessageBox::Ok);
}
