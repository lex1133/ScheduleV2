#include "xmlparser.h"

XMLParser::XMLParser()
{

}

void XMLParser::ReadXMLData(QFile* file)
{
    xml.setDevice(file);
    while (!xml.atEnd() && !xml.hasError() && xml.readNextStartElement())
    {
        if (xml.name() == "timetable")
        {
            while (xml.readNextStartElement())
            {
                if(xml.name() == "classes")
                    ReadClasses();
                else if(xml.name() == "subjects")
                    ReadSubjects();
                else if(xml.name() == "rooms")
                    ReadRooms();
                else if(xml.name() == "teachers")
                    ReadTeachers();
                else if(xml.name() == "study_types")
                    ReadStudyTypes();
                else if(xml.name() == "chairs")
                    ReadChairs();
                else if(xml.name() == "loads")
                    ReadLoads();
                else
                    xml.skipCurrentElement();
            }
        }
        else
        {
            xml.skipCurrentElement();
        }
    }
}

QHash<int, Items::ChairObj>* XMLParser::getChairs()
{
    return &chairs;
}

QHash<int, Items::TeacherObj> *XMLParser::getTeachers()
{
    return &teachers;
}

QHash<int, Items::RoomObj> *XMLParser::getRooms()
{
    return &rooms;
}

QHash<int, Items::SubjectObj> *XMLParser::getSubjects()
{
    return &subjects;
}

QHash<int, Items::ClassObj> *XMLParser::getClasses()
{
    return &classes;
}

void XMLParser::ReadClasses()
{
    Items::ClassObj classObj;
    int id = 0;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "classes"))
    {
        if (xml.name() == "id")
            id = xml.readElementText().toInt();
        if (xml.name() == "name")
            classObj.name = xml.readElementText();
        if (xml.name() == "student")
            classObj.students = xml.readElementText().toInt();
        if (xml.name() == "semester")
            classObj.semester = xml.readElementText().toInt();
        if (xml.name() == "work_hours")
        {
            classObj.work_hours.clear();
            xml.readNext();
            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "work_hours"))
            {
                if(xml.name() == "week")
                {
                    QVector<int> week;
                    week.clear();
                    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "week"))
                    {
                        if(xml.name() == "day")
                            week.push_back(xml.readElementText().toInt());
                        xml.readNext();
                    }
                    classObj.work_hours.push_back(week);
                }
                xml.readNext();
            }
        }
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "class"))
        {
            classes[id] = classObj;
        }
        xml.readNext();
    }

}

void XMLParser::ReadSubjects()
{
    Items::SubjectObj subjectObj;
    int id = 0;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "subjects"))
    {
        if (xml.name() == "id")
            id = xml.readElementText().toInt();
        if (xml.name() == "short_name")
            subjectObj.shortName = xml.readElementText();
        if (xml.name() == "full_name")
            subjectObj.fullName = xml.readElementText();
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "subject"))
        {
            subjects[id] = subjectObj;
        }
        xml.readNext();
    }
}

void XMLParser::ReadRooms()
{
    Items::RoomObj roomObj;
    int id = 0;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "rooms"))
    {
        if (xml.name() == "id")
            id = xml.readElementText().toInt();
        if (xml.name() == "name")
            roomObj.name = xml.readElementText();
        if (xml.name() == "capacity")
            roomObj.capacity = xml.readElementText().toInt();
        if (xml.name() == "chair_id")
            roomObj.chairId = xml.readElementText().toInt();
        if (xml.name() == "work_hours")
        {
            roomObj.work_hours.clear();
            xml.readNext();
            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "work_hours"))
            {
                if(xml.name() == "week")
                {
                    QVector<int> week;
                    week.clear();
                    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "week"))
                    {
                        if(xml.name() == "day")
                            week.push_back(xml.readElementText().toInt());
                        xml.readNext();
                    }
                    roomObj.work_hours.push_back(week);
                }
                xml.readNext();
            }
        }
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "room"))
        {
            rooms[id] = roomObj;
        }
        xml.readNext();
    }
}

void XMLParser::ReadTeachers()
{
    Items::TeacherObj teacherObj;
    int id = 0;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "teachers"))
    {
        if (xml.name() == "id")
            id = xml.readElementText().toInt();
        if (xml.name() == "surname")
            teacherObj.surname = xml.readElementText();
        if (xml.name() == "first_name")
            teacherObj.firstName = xml.readElementText();
        if (xml.name() == "second_name")
            teacherObj.secondName = xml.readElementText();
        if (xml.name() == "chair_id")
            teacherObj.chairId = xml.readElementText().toInt();
        if (xml.name() == "work_hours")
        {
            teacherObj.work_hours.clear();
            xml.readNext();
            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "work_hours"))
            {
                if(xml.name() == "week")
                {
                    QVector<int> week;
                    week.clear();
                    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "week"))
                    {
                        if(xml.name() == "day")
                            week.push_back(xml.readElementText().toInt());
                        xml.readNext();
                    }
                    teacherObj.work_hours.push_back(week);
                }
                xml.readNext();
            }
        }
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "teacher") && (teacherObj.firstName != "Fake" && teacherObj.surname[0] != '_' && teacherObj.surname != '='))
        {
            teachers[id] = teacherObj;
        }
        xml.readNext();
    }
}


void XMLParser::ReadStudyTypes()
{
    Items::StudyTypeObj studyTypeObj;
    int id = 0;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "study_types"))
    {
        if (xml.name() == "id")
            id = xml.readElementText().toInt();
        if (xml.name() == "short_name")
            studyTypeObj.shortName = xml.readElementText();
        if (xml.name() == "full_name")
            studyTypeObj.fullName = xml.readElementText();
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "study_type"))
        {
            studyTypes[id] = studyTypeObj;
        }
        xml.readNext();
    }
}

void XMLParser::ReadChairs()
{
    Items::ChairObj chairObj;
    int id = 0;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "chairs"))
    {
        if (xml.name() == "id")
            id = xml.readElementText().toInt();
        if (xml.name() == "short_name")
            chairObj.shortName = xml.readElementText();
        if (xml.name() == "full_name")
            chairObj.fullName = xml.readElementText();
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "chair"))
        {
            chairs[id] = chairObj;
        }
        xml.readNext();
    }
}

void XMLParser::ReadLoads()
{
    Items::LoadObj loadObj;
    int id = 0;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "loads"))
    {
        if (xml.name() == "id")
            id = xml.readElementText().toInt();
        if (xml.name() == "same_time")
            loadObj.sameTime = xml.readElementText().toInt();
        if (xml.name() == "groups")
        {
            loadObj.groups.clear();
            xml.readNext();
            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "groups"))
            {
                if(xml.name() == "group")
                {
                    Items::GroupObj group;
                    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "group"))
                    {
                        if(xml.name() == "teacher_id")
                            group.teacherId = xml.readElementText().toInt();
                        if(xml.name() == "subject_id")
                            group.subjectId = xml.readElementText().toInt();
                        if(xml.name() == "room_id_list")
                        {
                            QVector<int> roomIdList;
                            roomIdList.clear();
                            while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "room_id_list"))
                            {
                                if(xml.name() == "int")
                                    roomIdList.push_back(xml.readElementText().toInt());
                                xml.readNext();
                            }
                            group.roomIdList = roomIdList;
                        }
                        if(xml.name() == "hours_total")
                            group.hoursTotal = xml.readElementText().toInt();
                        if(xml.name() == "hours_per_week")
                            group.hoursPerWeek = xml.readElementText().toInt();
                        if(xml.name() == "week_type")
                            group.weekType = xml.readElementText();
                        if(xml.name() == "period_position")
                            group.periodPosition = xml.readElementText();
                        if(xml.name() == "pair_type")
                            group.pairType= xml.readElementText();
                        if(xml.name() == "study_type_id")
                            group.studyTypeId = xml.readElementText().toInt();
                        if(xml.name() == "hour_per_week_list")
                        {
                            QVector<int> hourPerWeekList;
                            hourPerWeekList.clear();
                            while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "hour_per_week_list"))
                            {
                                if(xml.name() == "int")
                                    hourPerWeekList.push_back(xml.readElementText().toInt());
                                xml.readNext();
                            }
                            group.hourPerWeekList = hourPerWeekList;
                        }
                        xml.readNext();
                    }
                    loadObj.groups.push_back(group);
                }
                xml.readNext();
            }
        }
        if(xml.name() == "klass_id_list")
        {
            QVector<int> klassIdList;
            klassIdList.clear();
            while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "klass_id_list"))
            {
                if(xml.name() == "int")
                    klassIdList.push_back(xml.readElementText().toInt());
                xml.readNext();
            }
            loadObj.klassIdList = klassIdList;
        }
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "load"))
        {
            loads[id] = loadObj;
        }
        xml.readNext();
    }
}

