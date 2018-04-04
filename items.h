#ifndef ITEMS_H
#define ITEMS_H
#include <QString>
#include <QList>

namespace Items {



struct ClassObj
{
    QString name;
    int students;
    QVector<QVector<int>> work_hours;
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
    int chairId;
    bool haveSocket;
    QVector<QVector<int>> work_hours;
    RoomObj(){}
};

struct TeacherObj
{
    QString surname;
    QString firstName;
    QString secondName;
    int chairId;
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

struct GroupObj
{
    int teacherId;
    int subjectId;
    QVector<int> roomIdList;
    int hoursTotal;
    int hoursPerWeek;
    QString weekType;
    QString periodPosition;
    QString pairType;
    int studyTypeId;
    QVector<int> hourPerWeekList;
    GroupObj() {}
};

struct LoadObj
{
    bool sameTime;
    QVector<GroupObj> groups;
    QVector<int> klassIdList;
    LoadObj() {}
};

struct SchedObj
{
    int day;
    int hour;
    int group;
    int loadId;
    int roomId;
    bool fixed;
    QString beginDate;
    QString endDate;
    SchedObj() {}
};

}
#endif // ITEMS_H
