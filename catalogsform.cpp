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
    loadTeachers();
    loadRooms();
    loadSubjects();
    loadClasses();
}

void CatalogsForm::loadChairs()
{
    ui->CatalogsChairsTable->setRowCount(parser->getChairs()->size());
    for (QHash<int,Items::ChairObj>::iterator i = parser->getChairs()->begin(); i != parser->getChairs()->end(); ++i)
    {
        ui->CatalogsChairsTable->setItem(i.key(),0, new QTableWidgetItem(i.value().fullName));
        ui->CatalogsChairsTable->setItem(i.key(),1, new QTableWidgetItem(i.value().shortName));
    }
}

void CatalogsForm::loadTeachers()
{
    ui->CatalogsTeachersTable->setRowCount(parser->getTeachers()->size());

    for (QHash<int,Items::TeacherObj>::iterator i = parser->getTeachers()->begin(); i != parser->getTeachers()->end(); ++i)
    {
        ui->CatalogsTeachersTable->setItem(i.key(),0, new QTableWidgetItem(i.value().surname + " " + i.value().firstName + ". " + i.value().secondName + "."));
        if(i.value().chairId != -1)
        {
            ui->CatalogsTeachersTable->setItem(i.key(),1, new QTableWidgetItem(parser->getChairs()->value(i.value().chairId).fullName));
        }
        else
        {
            ui->CatalogsTeachersTable->setItem(i.key(),1, new QTableWidgetItem("Не определена"));
        }


    }
}

void CatalogsForm::loadRooms()
{
    ui->CatalogsRoomsTable->setRowCount(parser->getRooms()->size());

    for (QHash<int,Items::RoomObj>::iterator i = parser->getRooms()->begin(); i != parser->getRooms()->end(); ++i)
    {
        ui->CatalogsRoomsTable->setItem(i.key(),0, new QTableWidgetItem(i.value().name));
        ui->CatalogsRoomsTable->setItem(i.key(),1, new QTableWidgetItem(QString::number(i.value().capacity)));
        if(i.value().chairId != -1)
        {
            ui->CatalogsRoomsTable->setItem(i.key(),2, new QTableWidgetItem(parser->getChairs()->value(i.value().chairId).fullName));
        }
        else
        {
            ui->CatalogsRoomsTable->setItem(i.key(),2, new QTableWidgetItem("Не определена"));
        }


    }
}

void CatalogsForm::loadSubjects()
{
    ui->CatalogsSubjectsTable->setRowCount(parser->getSubjects()->size());

    for (QHash<int,Items::SubjectObj>::iterator i = parser->getSubjects()->begin(); i != parser->getSubjects()->end(); ++i)
    {
        ui->CatalogsSubjectsTable->setItem(i.key(),0, new QTableWidgetItem(i.value().fullName));
        ui->CatalogsSubjectsTable->setItem(i.key(),1, new QTableWidgetItem(i.value().shortName));

    }
}

void CatalogsForm::loadClasses()
{
    ui->CatalogsClassesTable->setRowCount(parser->getClasses()->size());

    for (QHash<int,Items::ClassObj>::iterator i = parser->getClasses()->begin(); i != parser->getClasses()->end(); ++i)
    {
        ui->CatalogsClassesTable->setItem(i.key(),0, new QTableWidgetItem(i.value().name));
        ui->CatalogsClassesTable->setItem(i.key(),1, new QTableWidgetItem(QString::number(i.value().students)));
        ui->CatalogsClassesTable->setItem(i.key(),2, new QTableWidgetItem(QString::number(i.value().semester)));

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
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setColorMode(QPrinter::GrayScale);
    printer.setOrientation(QPrinter::Landscape);
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFileName(ui->CatalogsClassesTable->item(row,0)->text() + ".pdf");
    QPainter painter;
    if(!painter.begin(&printer))
    {
        qWarning("Falied");
    }
    drawSchedule(painter, printer.pageRect(),"class",row);


    painter.end();
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

    QVector<QString> hours {"8:30 - 10:10","10:20 - 12:00", "12:20 - 14:00", "14:10 - 15:50", "16:00 - 17:40", "18:00 - 19:30", "19:40 - 21:10", "21:20 - 22:50"};
    QFont f = QFont("Arial",v*0.65);
    painter.setFont(f);

    for (int i = 0; i < hours.size(); i++)
    {
        int h = i + 1;
        painter.drawText(verts[h], hors[0],
                verts[h + 1] - verts[h], hors[1] - hors[0],
                Qt::AlignVCenter | Qt::AlignCenter,
                hours[i]);

    }

    QVector<QString> daynamesFull {"Понедельник","Вторник","Среда","Четверг","Пятница","Суббота"};
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
    painter.drawText(verts[0], 20 * v,
            verts[verts.size() - 1] - verts[0], 40 *v,
            Qt::AlignVCenter | Qt::AlignCenter,
            title);
    if(type == "class")
        drawClassSched(painter, verts, hors, ui->CatalogsClassesTable->item(row,0)->text());

}


void CatalogsForm::drawClassSched(QPainter &painter, QList<int> &verts, QList<int> &hors, QString className)
{

    int classId = 0;
    for (QHash<int,Items::ClassObj>::iterator i = parser->getClasses()->begin(); i != parser->getClasses()->end(); ++i)
    {
        if(i.value().name == className)
        {
            classId = i.key();
            break;
        }
    }
    struct SubInfo
    {
        QString name;
        QString teacher;
        QString room;
        QVector<QPair<QDate,QDate>> dates;
        int type;
        QString group;
        int groupsNum;
        int lastRect;
        bool operator==(SubInfo const &right)
        {
            return (this->name == right.name && this->teacher == right.teacher && this->room == right.room &&
                    this->type == right.type && this->group == right.group &&
                    this->groupsNum == right.groupsNum);
        }

        void clear()
        {
            name.clear();
            teacher.clear();
            room.clear();
            dates.clear();
            type = NULL;
            group.clear();
            groupsNum = NULL;
            lastRect = 0;
        }
        SubInfo() {}
    };
    QVector<QVector<QPair<int,QVector<SubInfo>>>> schedObj;
    schedObj.resize(6);
    for(int i = 0; i < 6; i++)
        schedObj[i].resize(8);
    for (QHash<int,Items::SchedObj>::iterator i = parser->getScheds()->begin(); i != parser->getScheds()->end(); ++i)
    {
        if(parser->getLoads()->value(i.value().loadId).klassIdList.contains(classId))
        {
            SubInfo tmp;
            QDate beginDate = QDate::fromString(i.value().beginDate,"dd.MM.yyyy");
            QDate endDate = QDate::fromString(i.value().endDate,"dd.MM.yyyy");
            QString subject = parser->getSubjects()->value(parser->getLoads()->value(i.value().loadId).groups[0].subjectId).fullName;
            tmp.name = subject;
            tmp.type = parser->getLoads()->value(i.value().loadId).groups[0].studyTypeId;
            tmp.groupsNum = parser->getLoads()->value(i.value().loadId).groups.size();
            Items::TeacherObj teacherObj = parser->getTeachers()->value(parser->getLoads()->value(i.value().loadId).groups[0].teacherId);
            if(teacherObj.surname[0] != '_' && teacherObj.surname[0] != '=')
                tmp.teacher = teacherObj.surname + " " + teacherObj.firstName + "." + teacherObj.secondName + ".";
            if(parser->getLoads()->value(i.value().loadId).groups[0].roomIdList.size() > 0)
            {
                Items::RoomObj roomObj = parser->getRooms()->value(parser->getLoads()->value(i.value().loadId).groups[0].roomIdList[0]);
                tmp.room = roomObj.name;
            }
            QPair<QDate, QDate> date;
            date.first = beginDate.addDays(i.value().day-1);
            date.second = endDate.addDays(-(6-(i.value().day-1)));
            tmp.dates.push_back(date);
            tmp.group = i.value().group;
            bool f = false;
            for(int sub = 0; sub < schedObj[i.value().day-1][i.value().hour-1].second.size(); sub++)
            {
                if(schedObj[i.value().day-1][i.value().hour-1].second[sub] == tmp)
                {
                    schedObj[i.value().day-1][i.value().hour-1].second[sub].dates.push_back(tmp.dates[0]);
                    f = true;
                }
            }
            if(!f)
                schedObj[i.value().day-1][i.value().hour-1].second.push_back(tmp);
        }
    }
    QFont f;
    QTextOption opt;
    for(int i = 0; i < schedObj.size(); i++)
    {
        for(int j = 0; j < schedObj[i].size(); j++)
        {

            int daySize = schedObj[i][j].second.size();
            if(daySize == 1)
                f = QFont("Arial",13*0.50);
            else if(daySize == 2)
                f = QFont("Arial",13*0.45);
            else if(daySize > 2 && daySize < 5)
                f = QFont("Arial",13*0.40);
            else if(daySize >= 5)
                f = QFont("Arial",12*0.35);
            painter.setFont(f);
            opt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
            opt.setAlignment(Qt::AlignLeft);

            QFontMetrics fm(f);
            for(int k = 0; k < schedObj[i][j].second.size(); k++)
            {
                if(!schedObj[i][j].second[k].name.isEmpty())
                {
                    QString curSub;
                    curSub.clear();
                    std::sort(schedObj[i][j].second[k].dates.begin(),schedObj[i][j].second[k].dates.end(),
                              [](const QPair<QDate,QDate>& x,const QPair<QDate,QDate>& y) -> bool
                    {
                        return x.first < y.first;
                    }
                    );
                    curSub += schedObj[i][j].second[k].name + "." + schedObj[i][j].second[k].teacher + " " + parser->getStudyTypes()->value(schedObj[i][j].second[k].type).fullName
                            + (schedObj[i][j].second[k].type == 2 ?(schedObj[i][j].second[k].groupsNum > 1 ? (schedObj[i][j].second[k].group == 1 ? " (Б) " : " (А) ") : ""): "") + ". " + schedObj[i][j].second[k].room + ". " + "[";
                    for(int date = 0; date < schedObj[i][j].second[k].dates.size(); date++)
                    {
                        if(schedObj[i][j].second[k].dates[date].first.toString("dd.MM") != schedObj[i][j].second[k].dates[date].second.toString("dd.MM"))
                            curSub += schedObj[i][j].second[k].dates[date].first.toString("dd.MM") + " - " + schedObj[i][j].second[k].dates[date].second.toString("dd.MM");
                        else
                            curSub += schedObj[i][j].second[k].dates[date].first.toString("dd.MM");
                        if(date != schedObj[i][j].second[k].dates.size() -1)
                            curSub += ",";
                    }
                    curSub += "]";
                    if(schedObj[i][j].second[k].type == 2)
                    {

                        auto br = fm.tightBoundingRect(curSub);
                        painter.drawText(QRect(verts[1+j]+10,hors[1+i]+10+schedObj[i][j].first,verts[3]-verts[1]-20,hors[2]-hors[1]-20+schedObj[i][j].first),Qt::AlignLeft | Qt::TextWordWrap,curSub,&br);
                        schedObj[i][j].first += br.height();
                        if(schedObj[i].size()-1 >= j+1)
                        if(schedObj[i][j+1].second.size()-1 >= k)
                        {
                            schedObj[i][j+1].second[k].clear();
                            schedObj[i][j+1].first += br.height();
                        }
                        auto prePen = painter.pen();
                        painter.setPen(QPen(Qt::black,4));
                        br.setWidth(verts[3]-verts[1]);
                        painter.drawRect(br);
                        painter.setPen(prePen);
                    }
                    else
                    {

                        auto br = fm.tightBoundingRect(curSub);
                        painter.drawText(QRect(verts[1+j]+10,hors[1+i]+10+schedObj[i][j].first,verts[2]-verts[1]-20,hors[2]-hors[1]-20+schedObj[i][j].first),Qt::AlignLeft | Qt::TextWordWrap,curSub,&br);
                        schedObj[i][j].first += br.height();
                        auto prePen = painter.pen();
                        painter.setPen(QPen(Qt::black,4));
                        br.setWidth(verts[1+j+1]-verts[1+j]);
                        painter.drawRect(br);
                        painter.setPen(prePen);
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



void CatalogsForm::on_pushButton_clicked()
{

    for(int i = 0; i < ui->CatalogsClassesTable->rowCount(); i++)
    {
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setColorMode(QPrinter::GrayScale);
        printer.setOrientation(QPrinter::Landscape);
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFileName(ui->CatalogsClassesTable->item(i,0)->text() + ".pdf");
        QPainter painter;
        if(!painter.begin(&printer))
        {
            qWarning("Falied");
        }
        drawSchedule(painter, printer.pageRect(),"class",i);
        painter.end();
    }
}
