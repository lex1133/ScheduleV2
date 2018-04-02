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

private:

    void ReadClasses();
    void ReadSubjects();
    void ReadRooms();
    void ReadTeachers();
    void ReadStudyTypes();
    void ReadChairs();
    void ReadSpecialities();

    QXmlStreamReader xml;
    QHash<int,Items::ClassObj> classes;
    QHash<int,Items::SubjectObj> subjects;
    QHash<int,Items::RoomObj> rooms;
    QHash<int,Items::TeacherObj> teachers;
    QHash<int,Items::StudyTypeObj> studyTypes;
    QHash<int,Items::ChairObj> chairs;
    QHash<int,Items::SpecialityObj> specialities;
};

#endif // XMLPARSER_H
