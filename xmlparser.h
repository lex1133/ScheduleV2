#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "items.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>


class XMLParser
{
public:
    XMLParser();
    void ReadXMLData(QFile* file);
    QHash<int,Items::ChairObj>* getChairs();
    QHash<int,Items::TeacherObj>* getTeachers();
    QHash<int,Items::RoomObj>* getRooms();
    QHash<int,Items::SubjectObj>* getSubjects();
    QHash<int,Items::ClassObj>* getClasses();

private:

    void ReadClasses();
    void ReadSubjects();
    void ReadRooms();
    void ReadTeachers();
    void ReadStudyTypes();
    void ReadChairs();
    void ReadLoads();

    QXmlStreamReader xml;
    QHash<int,Items::ClassObj> classes;
    QHash<int,Items::SubjectObj> subjects;
    QHash<int,Items::RoomObj> rooms;
    QHash<int,Items::TeacherObj> teachers;
    QHash<int,Items::StudyTypeObj> studyTypes;
    QHash<int,Items::ChairObj> chairs;
    QHash<int,Items::LoadObj> loads;
};

#endif // XMLPARSER_H
