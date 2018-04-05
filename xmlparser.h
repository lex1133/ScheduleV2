#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "items.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QMessageBox>
#include <QDebug>


class XMLParser
{
public:
    XMLParser();
    bool ReadXMLData(QFile* file);

    Items::TermObj* getTerm();
    Items::TimesObj* getTimes();
    Items::HolidaysObj* getHolidays();
    QHash<int,Items::ChairObj>* getChairs();
    QHash<int,Items::TeacherObj>* getTeachers();
    QHash<int,Items::RoomObj>* getRooms();
    QHash<int,Items::SubjectObj>* getSubjects();
    QHash<int,Items::ClassObj>* getClasses();
    QHash<int,Items::LoadObj>* getLoads();
    QHash<int,Items::SchedObj>* getScheds();

private:

    void ReadTerm();
    void ReadTimes();
    void ReadHolidays();
    void ReadClasses();
    void ReadSubjects();
    void ReadRooms();
    void ReadTeachers();
    void ReadStudyTypes();
    void ReadChairs();
    void ReadLoads();
    void ReadScheds();

    QXmlStreamReader xml;    
    bool result = true;

    Items::TermObj term;
    Items::TimesObj times;
    Items::HolidaysObj holidays;
    QHash<int,Items::ClassObj> classes;
    QHash<int,Items::SubjectObj> subjects;
    QHash<int,Items::RoomObj> rooms;
    QHash<int,Items::TeacherObj> teachers;
    QHash<int,Items::StudyTypeObj> studyTypes;
    QHash<int,Items::ChairObj> chairs;
    QHash<int,Items::LoadObj> loads;
    QHash<int,Items::SchedObj> scheds;
};

#endif // XMLPARSER_H
