#include "catalogsform.h"
#include "ui_catalogsform.h"

CatalogsForm::CatalogsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CatalogsForm)
{
    ui->setupUi(this);
    ui->CatalogsTeachersTable->setColumnHidden(0,true);
}

CatalogsForm::~CatalogsForm()
{
    delete ui;
}

bool CatalogsForm::loadCatalogs(QSqlDatabase* db_,QSqlQuery* query_)
{
    this->db = db_;
    this->query = query_;
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
        ui->CatalogsChairsTable->setItem(query->value(0).toInt(),0, new QTableWidgetItem(query->value(1).toString()));
        ui->CatalogsChairsTable->setItem(query->value(0).toInt(),1, new QTableWidgetItem(query->value(2).toString()));
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
        ui->CatalogsRoomsTable->setItem(query->value(0).toInt(),0, new QTableWidgetItem(query->value(1).toString()));
        ui->CatalogsRoomsTable->setItem(query->value(0).toInt(),1, new QTableWidgetItem(QString::number(query->value(2).toInt())));
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
            ui->CatalogsRoomsTable->setItem(query->value(0).toInt(),2, new QTableWidgetItem(tmp.value(1).toString()));
        }
        else
        {
            ui->CatalogsRoomsTable->setItem(query->value(0).toInt(),2, new QTableWidgetItem("Не определена"));
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
        ui->CatalogsClassesTable->setItem(query->value(0).toInt(),0, new QTableWidgetItem(query->value(1).toString()));
        ui->CatalogsClassesTable->setItem(query->value(0).toInt(),1, new QTableWidgetItem(QString::number(query->value(2).toInt())));
        ui->CatalogsClassesTable->setItem(query->value(0).toInt(),2, new QTableWidgetItem(QString::number(query->value(3).toInt())));
    }

}




void CatalogsForm::on_CatalogsChairsTable_cellDoubleClicked(int row, int column)
{

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

        QTableWidgetItem *item = ui->CatalogsClassesTable->item( i, 0 );
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

        QTableWidgetItem *item = ui->CatalogsRoomsTable->item( i, 0 );
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


        QTableWidgetItem *item = ui->CatalogsTeachersTable->item( i, 0 );
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
    if(!d.exists("..\\Расписание\\Группы\\"))
        d.mkpath("..\\Расписание\\Группы\\");
    printer.setOutputFileName("..\\Расписание\\Группы\\" + ui->CatalogsClassesTable->item(row,0)->text() + ".pdf");
    QPainter painter;
    if(!painter.begin(&printer))
    {
        qWarning("Falied");
        return;
    }

    drawSchedule(painter, printer.pageRect(),"class",row);

    painter.end();


    qDebug()<<"Time elapsed: "<<timer.elapsed();
}

void CatalogsForm::drawSchedule(QPainter &painter,QRect pageRect, QString type, int row)
{
    float v =13;
    int padding = 20;
    QList<int> verts;
    QList<int> hors;
    v = pageRect.width() / 1000;
    padding = v * 1.5;
    verts = QList<int>();
    hors = QList<int>();
    QPen pen;
    pen.setWidth(15);
    painter.setPen(pen);
    QVector<QString> hours {"8:30 - 10:10","10:20 - 12:00", "12:20 - 14:00", "14:10 - 15:50", "16:00 - 17:40", "18:00 - 19:30", "19:40 - 21:10", "21:20 - 22:50"};
    QVector<QString> daynamesFull {"Понедельник","Вторник","Среда","Четверг","Пятница","Суббота"};
    QFont f = QFont("Arial",v*0.65);

    verts.append(30 * v);
    verts.append(verts.last() + 15*v);
    for (int i = 0; i < 8; i++)
    {
        verts.append(verts.last() + 120*v);
    }

    hors.append(70 * v);
    hors.append(hors.last() + 15*v);
    for (int i = 0; i < 6; i++)
    {
        hors.append(hors.last() + 100*v);
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
        title = ui->CatalogsClassesTable->item(row,0)->text();
    else if(type == "room")
        title = ui->CatalogsRoomsTable->item(row,0)->text();
    else if(type == "teacher")
        title = ui->CatalogsTeachersTable->item(row,0)->text();
    painter.drawText(verts[0], 20 * v,
            verts[verts.size() - 1] - verts[0], 40 *v,
            Qt::AlignVCenter | Qt::AlignCenter,
            title);
    if(type == "class")
        drawClassSched(painter, verts, hors, ui->CatalogsClassesTable->item(row,0)->text());
    if(type == "teacher")
        drawTeacherSched(painter, verts, hors, row);

}


void CatalogsForm::drawClassSched(QPainter &painter, QList<int> &verts, QList<int> &hors, QString className)
{
    db->transaction();
    int classId = 0;
    query->prepare("SELECT * FROM `Classes` WHERE name=:name");
    query->bindValue(":name",className);
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
    if(numberOfRows != 0)
    {
        query->next();
        classId = query->value(0).toInt();
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
        QDate beginDate = QDate::fromString(query->value(6).toString(),"dd.MM.yyyy");
        QDate endDate = QDate::fromString(query->value(7).toString(),"dd.MM.yyyy");

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
    query->prepare("SELECT * FROM `Scheds` WHERE loadId IN (select loadId from `LoadGroups` where teacherId == "+ ui->CatalogsTeachersTable->item(teacherId,0)->text() + ")");
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
        QDate beginDate = QDate::fromString(query->value(6).toString(),"dd.MM.yyyy");
        QDate endDate = QDate::fromString(query->value(7).toString(),"dd.MM.yyyy");

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

    QFont f;
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
            int daySize = schedObj[i][j].second.size();
            if(schedObj[i][j].first > (hors[2]-hors[1])/3 && daySize > 2 && daySize < 4)
                f = QFont("Arial",12*0.35);
            else if(schedObj[i][j].first > (hors[2]-hors[1])/3 && daySize >= 4)
                f = QFont("Arial",12*0.25);
            else if(schedObj[i][j].first > (hors[2]-hors[1])/2 && daySize >= 3)
                f = QFont("Arial",12*0.25);
            else if(schedObj[i][j].first > (hors[2]-hors[1])/2 && daySize == 1)
                f = QFont("Arial",13*0.35);
            else if(schedObj[i][j].first > (hors[2]-hors[1])/2 && daySize == 2)
                f = QFont("Arial",13*0.25);
            else if(daySize == 1)
                f = QFont("Arial",13*0.50);
            else if(daySize == 2)
                f = QFont("Arial",13*0.45);
            else if(daySize > 2 && daySize < 5)
                f = QFont("Arial",12*0.40);
            else if(daySize >= 5)
                f = QFont("Arial",12*0.35);


            painter.setFont(f);


            for(int k = 0; k < schedObj[i][j].second.size(); k++)
            {
                if(!schedObj[i][j].second[k].name.isEmpty())
                {
                    if(k == 0 && schedObj[i][j].first != 0)
                    {
                        painter.drawLine(verts[2+j],hors[1+i],verts[2+j],hors[1+i]+schedObj[i][j].first+10);
                        painter.drawLine(verts[2+j],hors[1+i]+schedObj[i][j].first+10,verts[j],hors[1+i]+schedObj[i][j].first+10);
                        if(schedObj[i][j].second[k].type == 2 && schedObj[i][j].second[k].pair == true)
                            painter.drawLine(verts[2+j],hors[1+i]+schedObj[i][j].first+10,verts[j+3],hors[1+i]+schedObj[i][j].first+10);
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

                    curSub += schedObj[i][j].second[k].name + "." + schedObj[i][j].second[k].teacher + " " + StudyTypeObj.value(1).toString()
                            + (schedObj[i][j].second[k].type == 2 ?(schedObj[i][j].second[k].groupsNum > 1 ? (schedObj[i][j].second[k].group == QString::number(1) ? " (Б) " : " (А) ") : ""): "") + ". " + schedObj[i][j].second[k].room + ". " + "[";
                    for(int date = 0; date < schedObj[i][j].second[k].dates.size(); date++)
                    {
                        if(schedObj[i][j].second[k].dates[date].first.toString("dd.MM") != schedObj[i][j].second[k].dates[date].second.toString("dd.MM"))
                            curSub += schedObj[i][j].second[k].dates[date].first.toString("dd.MM") + " - " + schedObj[i][j].second[k].dates[date].second.toString("dd.MM") + " к.н.";
                        else
                            curSub += schedObj[i][j].second[k].dates[date].first.toString("dd.MM");
                        if(date != schedObj[i][j].second[k].dates.size() -1)
                            curSub += ",";
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
                                    drawArea = QRect(verts[1+j]+10,hors[1+i]+10+schedObj[i][j].first,verts[3]-verts[1]-20,hors[2]-hors[1]-20+schedObj[i][j].first);
                                    break;
                                }
                            }
                        }
                        if(!complex)
                        {
                            drawArea = QRect(verts[1+j]+10,hors[1+i]+10+schedObj[i][j].first,verts[2]-verts[1]-20,hors[2]-hors[1]-20+schedObj[i][j].first);
                        }


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

QFont CatalogsForm::fillRect(QString& str,QRect &rect, int flags, QFont &font)
{
    QFont result = font;
    QPainter painter(this);
    painter.setFont(result);
    QRect br;
    while(br.height() > rect.height())
    {
        painter.drawText(rect,flags | Qt::TextDontPrint,str,&br);
        result.setPointSizeF(result.pointSizeF()*0.95);
        painter.setFont(result);
    }
    return result;
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
        if(!d.exists("..\\Расписание\\Группы\\"))
            d.mkpath("..\\Расписание\\Группы\\");
        printer.setOutputFileName("..\\Расписание\\Группы\\" + ui->CatalogsClassesTable->item(i,0)->text() + ".pdf");
        QPainter painter;
        if(!painter.begin(&printer))
        {
            qWarning("Falied");
        }

        drawSchedule(painter, printer.pageRect(),"class",i);

        painter.end();
    }

    progress.setValue(ui->CatalogsClassesTable->rowCount());
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
    if(!d.exists("..\\Расписание\\Преподаватели\\"))
        d.mkpath("..\\Расписание\\Преподаватели\\");
    printer.setOutputFileName("..\\Расписание\\Преподаватели\\" + ui->CatalogsTeachersTable->item(row,1)->text() + ".pdf");
    QPainter painter;
    if(!painter.begin(&printer))
    {
        qWarning("Falied");
        return;
    }

    drawSchedule(painter, printer.pageRect(),"teacher",row);

    painter.end();


    qDebug()<<"Time elapsed: "<<timer.elapsed();
}

void CatalogsForm::on_ExportAllTeachersButton_clicked()
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
        if(!d.exists("..\\Расписание\\Преподаватели\\"))
            d.mkpath("..\\Расписание\\Преподаватели\\");
        printer.setOutputFileName("..\\Расписание\\Преподаватели\\" + ui->CatalogsTeachersTable->item(i,1)->text() + ".pdf");
        QPainter painter;
        if(!painter.begin(&printer))
        {
            qWarning("Falied");
            return;
        }

        drawSchedule(painter, printer.pageRect(),"teacher",i);

        painter.end();
    }

    progress.setValue(ui->CatalogsClassesTable->rowCount());
}
