#include "xmlparser.h"

XMLParser::XMLParser()
{

}

bool XMLParser::GenerateBase()
{
    auto fileName = QFileDialog::getSaveFileName(0,
                                                 QObject::tr("Сохранить новую базу данных"), "" , QObject::tr("SCDB файл (*.scdb)"));
    if(!fileName.isEmpty())
    {
        QFileInfo check_file(fileName);
        if (check_file.exists() && check_file.isFile())
        {
            QFile::remove(fileName);
        }

        db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
        db->setDatabaseName(check_file.absoluteFilePath());
        newbase = check_file.absoluteFilePath();

        if (db->open()) {
            qDebug()<<"[+] Connected to Database File";
        }
        else {
            qDebug()<<"[!] Database File was not opened";
        }

        query = new QSqlQuery();


        query->exec("PRAGMA schema.journal_mode = MEMORY");
        query->exec("PRAGMA schema.synchronous = OFF");
        db->transaction();
        query->exec("CREATE TABLE `Reserved` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `room` INTEGER NOT NULL, `week` INTEGER NOT NULL, `day` INTEGER NOT NULL, `hour` INTEGER NOT NULL, `teacher` TEXT NOT NULL, `class` TEXT, `reason` TEXT )");
        query->exec("CREATE TABLE `Chairs` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `fullName` TEXT NOT NULL, `shortName` TEXT NOT NULL )");
        query->exec("CREATE TABLE \"Classes\" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `name` TEXT NOT NULL, `student` INTEGER NOT NULL, `semester` INTEGER NOT NULL, `work_hours` TEXT )");
        query->exec("CREATE TABLE `Holidays` ( `day` TEXT )");
        query->exec("CREATE TABLE `Loads` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `klassIdList` TEXT)");
        query->exec("CREATE TABLE \"Rooms\" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `name` TEXT NOT NULL, `capacity` INTEGER NOT NULL, `chairId` INTEGER NOT NULL, `work_hours` TEXT )");
        query->exec("CREATE TABLE `Scheds` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `day` INTEGER NOT NULL, `hour` INTEGER NOT NULL, `group` INTEGER NOT NULL, `loadId` INTEGER NOT NULL, `roomId` INTEGER NOT NULL, `beginDate` TEXT NOT NULL, `endDate` TEXT NOT NULL )");
        query->exec("CREATE TABLE `StudyTypes` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `fullName` TEXT NOT NULL, `shortName` TEXT NOT NULL )");
        query->exec("CREATE TABLE `Subjects` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `fullName` TEXT NOT NULL, `shortName` TEXT NOT NULL )");
        query->exec("CREATE TABLE \"Teachers\" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `surname` TEXT NOT NULL, `firstName` TEXT, `secondName` TEXT, `chairId` INTEGER, `work_hours` TEXT )");
        query->exec("CREATE TABLE `Term` ( `beginDate` TEXT NOT NULL, `endDate` TEXT NOT NULL )");
        query->exec("CREATE TABLE `Times` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `time` TEXT NOT NULL )");
        query->exec("CREATE TABLE `LoadGroups` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,`loadId` INTEGER NOT NULL, `teacherId` INTEGER NOT NULL, `subjectId` INTEGER NOT NULL, `roomIdList` TEXT, `studyTypeId` INTEGER NOT NULL )");
        if(!db->commit())
        {
            qDebug()<<"Error";
            return false;
        }
        return true;
    }
    return false;
}

bool XMLParser::ReadXMLData(QFile* file,QSqlDatabase* db_,QSqlQuery* query_)
{
    this->db = db_;
    this->query = query_;
    xml.setDevice(file);
    if(!GenerateBase())
        return false;

    db->transaction();
    while (!xml.atEnd() && xml.readNextStartElement())
    {
        if (xml.name() == "timetable")
        {
            result = true;
            while (xml.readNextStartElement())
            {
                if(xml.hasError()) {
                    result = false;
                    break;
                }
                if(xml.name() == "times")
                {
                    ReadTimes();
                }
                else if(xml.name() == "holidays")
                {
                    ReadHolidays();
                }
                else if(xml.name() == "term")
                {
                    ReadTerm();
                }
                else if(xml.name() == "classes")
                {
                    ReadClasses();
                }
                else if(xml.name() == "subjects")
                {
                    ReadSubjects();
                }
                else if(xml.name() == "rooms")
                {
                    ReadRooms();
                }
                else if(xml.name() == "teachers")
                {
                    ReadTeachers();
                }
                else if(xml.name() == "study_types")
                {
                    ReadStudyTypes();
                }
                else if(xml.name() == "chairs")
                {
                    ReadChairs();
                }
                else if(xml.name() == "loads")
                {
                    ReadLoads();
                }
                else if(xml.name() == "scheds")
                {
                    ReadScheds();
                }
                else
                    xml.skipCurrentElement();
            }
        }
        else
        {
            xml.skipCurrentElement();
        }
    }
    if(!db->commit())
        qDebug()<<"Error";
    query->finish();

    xml.clear();
    file->close();
    if(!result)
    {
        QMessageBox::critical(nullptr,
                              "Ошибка чтения XML-файла.","Не правильный формат расписания",
                              QMessageBox::Ok);
    }
    return result;
}

void XMLParser::ReadTerm()
{
    query->prepare("INSERT INTO Term (beginDate, endDate) VALUES (:beginDate, :endDate)");
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "term"))
    {
        if(xml.hasError()) {
            result = false;
            break;
        }
        if (xml.name() == "begin_date")
            query->bindValue(":beginDate", xml.readElementText());
        if (xml.name() == "end_date")
            query->bindValue(":endDate", xml.readElementText());
        xml.readNext();
    }
    if(!query->exec())
    {
        qDebug()<<"[-] Wrong query";

        qDebug()<<query->lastError();
    }

}

void XMLParser::ReadTimes()
{
    query->prepare("INSERT INTO Times (time) "
                   "VALUES (:time)");
    if(xml.name() == "times")
    {
        while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "times"))
        {
            if(xml.name() == "time")
            {
                query->bindValue(":time", xml.readElementText());
                if(!query->exec())
                {
                    qDebug()<<"[-] Wrong query";

                    qDebug()<<query->lastError();
                }

            }
            xml.readNext();
        }
    }

}

void XMLParser::ReadHolidays()
{
    query->prepare("INSERT INTO Holidays (day) "
                   "VALUES (:day)");
    if(xml.name() == "holidays")
    {
        while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "holidays"))
        {
            if(xml.name() == "day")
            {
                query->bindValue(":day", xml.readElementText());
                if(!query->exec())
                {
                    qDebug()<<"[-] Wrong query";

                    qDebug()<<query->lastError();
                }

            }
            xml.readNext();
        }
    }


}

void XMLParser::ReadClasses()
{
    query->prepare("INSERT INTO Classes (id, name, student, semester, work_hours) "
                   "VALUES (:id, :name, :student, :semester, :work_hours)");
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "classes"))
    {
        if(xml.hasError()) {
            result = false;
            break;
        }
        if (xml.name() == "id")
            query->bindValue(":id", xml.readElementText().toInt());
        if (xml.name() == "name")
            query->bindValue(":name", xml.readElementText());
        if (xml.name() == "student")
            query->bindValue(":student", xml.readElementText().toInt());
        if (xml.name() == "semester")
            query->bindValue(":semester", xml.readElementText().toInt());
        if (xml.name() == "work_hours")
        {
            QString work_hours;
            xml.readNext();
            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "work_hours"))
            {
                if(xml.hasError()) {
                    result = false;
                    break;
                }
                if(xml.name() == "week")
                {
                    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "week"))
                    {
                        if(xml.name() == "day")
                            work_hours += xml.readElementText() + " ";
                        xml.readNext();
                    }
                }
                xml.readNext();
            }
            query->bindValue(":work_hours", work_hours);
        }
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "class"))
        {
            if(!query->exec())
            {
                qDebug()<<"[-] Wrong query";

                qDebug()<<query->lastError();
            }

        }
        xml.readNext();
    }

}

void XMLParser::ReadSubjects()
{
    query->prepare("INSERT INTO Subjects (id, fullName, shortName) "
                   "VALUES (:id, :fullName, :shortName)");
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "subjects"))
    {
        if(xml.hasError()) {
            result = false;
            break;
        }
        if (xml.name() == "id")
            query->bindValue(":id", xml.readElementText().toInt());
        if (xml.name() == "short_name")
            query->bindValue(":shortName", xml.readElementText());
        if (xml.name() == "full_name")
            query->bindValue(":fullName", xml.readElementText());
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "subject") )
        {
            if(!query->exec())
            {
                qDebug()<<"[-] Wrong query";

                qDebug()<<query->lastError();
            }

        }
        xml.readNext();
    }
}

void XMLParser::ReadRooms()
{
    query->prepare("INSERT INTO Rooms (id, name, capacity, chairId, work_hours) "
                   "VALUES (:id, :name, :capacity, :chairId, :work_hours)");
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "rooms"))
    {
        if(xml.hasError()) {
            result = false;
            break;
        }
        if (xml.name() == "id")
            query->bindValue(":id", xml.readElementText().toInt());
        if (xml.name() == "name")
            query->bindValue(":name", xml.readElementText());
        if (xml.name() == "capacity")
            query->bindValue(":capacity", xml.readElementText().toInt());
        if (xml.name() == "chair_id")
            query->bindValue(":chairId", xml.readElementText().toInt());
        if (xml.name() == "work_hours")
        {
            QString work_hours;
            xml.readNext();
            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "work_hours"))
            {
                if(xml.hasError()) {
                    result = false;
                    break;
                }
                if(xml.name() == "week")
                {
                    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "week"))
                    {
                        if(xml.name() == "day")
                            work_hours += xml.readElementText() + " ";
                        xml.readNext();
                    }
                }
                xml.readNext();
            }
            query->bindValue(":work_hours", work_hours);
        }
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "room"))
        {
            if(!query->exec())
            {
                qDebug()<<"[-] Wrong query";

                qDebug()<<query->lastError();
            }

        }
        xml.readNext();
    }
}

void XMLParser::ReadTeachers()
{
    query->prepare("INSERT INTO Teachers (id, surname, firstName, secondName, chairId, work_hours) "
                   "VALUES (:id, :surname, :firstName, :secondName, :chairId, :work_hours)");
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "teachers"))
    {
        if(xml.hasError()) {
            result = false;
            break;
        }
        if (xml.name() == "id")
            query->bindValue(":id", xml.readElementText().toInt());
        if (xml.name() == "surname")
            query->bindValue(":surname", xml.readElementText());
        if (xml.name() == "first_name")
            query->bindValue(":firstName", xml.readElementText());
        if (xml.name() == "second_name")
            query->bindValue(":secondName", xml.readElementText());
        if (xml.name() == "chair_id")
            query->bindValue(":chairId", xml.readElementText().toInt());
        if (xml.name() == "work_hours")
        {
            QString work_hours;
            xml.readNext();
            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "work_hours"))
            {
                if(xml.hasError()) {
                    result = false;
                    break;
                }
                if(xml.name() == "week")
                {
                    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "week"))
                    {
                        if(xml.name() == "day")
                            work_hours += xml.readElementText() + " ";
                        xml.readNext();
                    }
                }
                xml.readNext();
            }
            query->bindValue(":work_hours", work_hours);
        }
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "teacher"))
        {
            if(!query->exec())
            {
                qDebug()<<"[-] Wrong query";

                qDebug()<<query->lastError();
            }

        }
        xml.readNext();
    }

}

void XMLParser::ReadStudyTypes()
{
    query->prepare("INSERT INTO StudyTypes (id, fullName, shortName) "
                   "VALUES (:id, :fullName, :shortName)");
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "study_types"))
    {
        if(xml.hasError()) {
            result = false;
            break;
        }
        if (xml.name() == "id")
            query->bindValue(":id", xml.readElementText().toInt());
        if (xml.name() == "short_name")
            query->bindValue(":shortName", xml.readElementText());
        if (xml.name() == "full_name")
            query->bindValue(":fullName", xml.readElementText());
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "study_type") )
        {
            if(!query->exec())
            {
                qDebug()<<"[-] Wrong query";

                qDebug()<<query->lastError();
            }
        }
        xml.readNext();
    }
}

void XMLParser::ReadChairs()
{
    query->prepare("INSERT INTO Chairs (id, fullName, shortName) "
                   "VALUES (:id, :fullName, :shortName)");
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "chairs"))
    {
        if(xml.hasError()) {
            result = false;
            break;
        }
        if (xml.name() == "id")
            query->bindValue(":id", xml.readElementText().toInt());
        if (xml.name() == "short_name")
            query->bindValue(":shortName", xml.readElementText());
        if (xml.name() == "full_name")
            query->bindValue(":fullName", xml.readElementText());
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "chair"))
        {
            if(!query->exec())
            {
                qDebug()<<"[-] Wrong query";

                qDebug()<<query->lastError();
            }
        }
        xml.readNext();
    }
}

void XMLParser::ReadLoads()
{
    query->prepare("INSERT INTO Loads (id, klassIdList) "
                   "VALUES (:id, :klassIdList)");

    int loadId = 0;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "loads"))
    {
        if(xml.hasError()) {
            result = false;
            break;
        }
        if (xml.name() == "id")
        {
            loadId = xml.readElementText().toInt();
            query->bindValue(":id", loadId);

        }
        if (xml.name() == "groups")
        {
            xml.readNext();
            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "groups"))
            {
                if(xml.hasError()) {
                    result = false;
                    break;
                }
                if(xml.name() == "group")
                {
                    QSqlQuery query2 = QSqlQuery();
                    query2.prepare("INSERT INTO LoadGroups (loadId, teacherId, subjectId, roomIdList, studyTypeId) "
                                   "VALUES (:loadId, :teacherId, :subjectId, :roomIdList, :studyTypeId)");
                    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "group"))
                    {
                        if(xml.hasError()) {
                            result = false;
                            break;
                        }
                        if(xml.name() == "teacher_id")
                            query2.bindValue(":teacherId", xml.readElementText().toInt());
                        if(xml.name() == "subject_id")
                            query2.bindValue(":subjectId", xml.readElementText().toInt());
                        if(xml.name() == "room_id_list")
                        {
                            QString roomIdList;
                            roomIdList.clear();
                            while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "room_id_list"))
                            {
                                if(xml.name() == "int")
                                    roomIdList += xml.readElementText() + " ";
                                xml.readNext();
                            }
                            query2.bindValue(":roomIdList", roomIdList);
                        }
                        if(xml.name() == "study_type_id")
                            query2.bindValue(":studyTypeId", xml.readElementText().toInt());

                        query2.bindValue(":loadId", loadId);
                        xml.readNext();
                        if(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "group")
                            if(!query2.exec())
                            {
                                qDebug()<<"[-] Wrong query";
                                qDebug()<<query2.lastError();
                            }
                    }
                }

                xml.readNext();
            }
        }
        if(xml.name() == "klass_id_list")
        {
            QString klassIdList;
            klassIdList.clear();
            klassIdList += " ";
            while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "klass_id_list"))
            {
                if(xml.hasError()) {
                    result = false;
                    break;
                }
                if(xml.name() == "int")
                    klassIdList += xml.readElementText() + " ";
                xml.readNext();
            }
            query->bindValue(":klassIdList", klassIdList);
        }
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "load"))
        {
            if(!query->exec())
            {
                qDebug()<<"[-] Wrong query";

                qDebug()<<query->lastError();
            }
        }
        xml.readNext();
    }
}

void XMLParser::ReadScheds()
{
    query->prepare("INSERT INTO `Scheds` (`id`, `day`, `hour`, `group`, `loadId`, `roomId`, `beginDate`, `endDate`) "
                   "VALUES (:id, :day, :hour, :group, :loadId, :roomId, :beginDate, :endDate)");
    int id = 0;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "scheds"))
    {
        if(xml.hasError()) {
            result = false;
            break;
        }
        if (xml.name() == "day")
        {
            int day = xml.readElementText().toInt();
            query->bindValue(":day", day);
        }
        if (xml.name() == "hour")
        {
            int hour = xml.readElementText().toInt();
            query->bindValue(":hour", hour);
        }
        if (xml.name() == "group")
        {
            int group = xml.readElementText().toInt();
            query->bindValue(":group", group);
        }
        if (xml.name() == "load_id")
        {
            int load_id = xml.readElementText().toInt();
            query->bindValue(":loadId", load_id);
        }
        if (xml.name() == "room_id")
        {
            int room_id = xml.readElementText().toInt();
            query->bindValue(":roomId", room_id);
        }
        if (xml.name() == "begin_date")
        {
            QString beginDate = xml.readElementText();
            QDate bd = QDate::fromString(beginDate,"dd.MM.yyyy");
            query->bindValue(":beginDate", bd.toString("yyyyMMdd"));
        }
        if (xml.name() == "end_date")
        {
            QString endDate = xml.readElementText();
            QDate ed = QDate::fromString(endDate,"dd.MM.yyyy");
            query->bindValue(":endDate", ed.toString("yyyyMMdd"));
        }
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "sched"))
        {
            query->bindValue(":id", id);
            if(!query->exec())
            {
                qDebug()<<"[-] Wrong query";

                qDebug()<<query->lastQuery();
                auto map = query->boundValues();

                qDebug()<<query->lastError();
            }
            id++;
        }
        xml.readNext();
    }
}






