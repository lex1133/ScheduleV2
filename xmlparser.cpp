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
                else if(xml.name() == "specialities>")
                    ReadSpecialities();
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
        if (xml.name() == "session")
            classObj.session = xml.readElementText().toInt();
        if (xml.name() == "student")
            classObj.students = xml.readElementText().toInt();
        if (xml.name() == "min_lessons_per_day")
            classObj.min_lessons = xml.readElementText().toInt();
        if (xml.name() == "max_lessons_per_day")
            classObj.max_lessons = xml.readElementText().toInt();
        if (xml.name() == "speciality_id")
            classObj.speciality_id = xml.readElementText().toInt();
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
        if (xml.name() == "building")
            roomObj.building = xml.readElementText().toInt();
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
            teacherObj.SecondName = xml.readElementText();
        if (xml.name() == "comment")
            teacherObj.comment = xml.readElementText();
        if (xml.name() == "class_id")
            teacherObj.classId = xml.readElementText().toInt();
        if (xml.name() == "subject_id")
            teacherObj.subjectId = xml.readElementText().toInt();
        if (xml.name() == "chair_id")
            teacherObj.chairId = xml.readElementText().toInt();
        if (xml.name() == "method_days")
            teacherObj.methodDays = xml.readElementText().toInt();
        if (xml.name() == "max_windows")
            teacherObj.maxWindows = xml.readElementText().toInt();
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
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "teacher"))
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

void XMLParser::ReadSpecialities()
{
    Items::SpecialityObj specialityObj;
    int id = 0;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "specialities"))
    {
        if (xml.name() == "id")
            id = xml.readElementText().toInt();
        if (xml.name() == "short_name")
            specialityObj.shortName = xml.readElementText();
        if (xml.name() == "full_name")
            specialityObj.fullName = xml.readElementText();
        if((xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "speciality"))
        {
            specialities[id] = specialityObj;
        }
        xml.readNext();
    }
}

