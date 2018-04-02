#ifndef ITEMS_H
#define ITEMS_H
#include <QString>
#include <QList>

namespace Items {



struct ClassObj
{
    QString name;
    int session;
    int students;
    int min_lessons;
    int max_lessons;
    QVector<QVector<int>> work_hours;
    int speciality_id;
    int semester;
    ClassObj(){}
};

struct SubjectObj
{
    QString shortName;
    QString fullName;
    SubjectObj(){}
};

struct RoomObj
{
    QString name;
    int capacity;
    int building;
    int chairId;
    QVector<QVector<int>> work_hours;
    RoomObj(){}
};

struct TeacherObj
{
    QString surname;
    QString firstName;
    QString SecondName;
    QString comment;
    int classId;
    int subjectId;
    int roomId;
    int chairId;
    int methodDays;
    int maxWindows;
    QVector<QVector<int>> work_hours;
    TeacherObj(){}
};

struct StudyTypeObj
{
    QString shortName;
    QString fullName;
    StudyTypeObj() {}
};

struct ChairObj
{
    QString shortName;
    QString fullName;
    ChairObj() {}
};

struct SpecialityObj
{
    QString shortName;
    QString fullName;
    SpecialityObj() {}
};
}
#endif // ITEMS_H
