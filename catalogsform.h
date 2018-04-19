#ifndef CATALOGSFORM_H
#define CATALOGSFORM_H

#include "xmlparser.h"

#include <QProgressDialog>
#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QPainter>
#include <QPrinter>
#include <QDate>
#include <QScreen>
#include <QDir>
#include <math.h>

namespace Ui {
class CatalogsForm;
}

class CatalogsForm : public QWidget
{
    Q_OBJECT

public:
    explicit CatalogsForm(QWidget *parent = 0);
    ~CatalogsForm();
    bool loadCatalogs(QSqlDatabase* db_,QSqlQuery* query_);

private:
    Ui::CatalogsForm *ui;    
    QSqlDatabase* db;
    QSqlQuery* query;
    bool loadResult = true;

    struct SubInfo
    {
        QString name;
        QString teacher;
        QString room;
        QVector<QPair<QDate,QDate>> dates;
        int type;
        QString group;
        int groupsNum;
        bool pair = false;
        bool equal(SubInfo const &right)
        {
            return (this->name == right.name && this->teacher == right.teacher && this->room == right.room &&
                    this->type == right.type && this->group == right.group &&
                    this->groupsNum == right.groupsNum);
        }

        bool operator==(SubInfo const &right)
        {
            return (this->name == right.name && this->teacher == right.teacher && this->room == right.room &&
                    this->dates == right.dates && this->type == right.type && this->group == right.group &&
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
            pair = false;
            groupsNum = NULL;
        }
        SubInfo() {}
    };


    void loadChairs();
    void loadTeachers();
    void loadRooms();
    void loadSubjects();
    void loadClasses();
    void drawClassSched(QPainter &painter, QList<int> &verts, QList<int> &hors, QString className);
    void drawTeacherSched(QPainter &painter, QList<int> &verts, QList<int> &hors, int teacherId);
    void drawSchedTable(QPainter &painter, QList<int> &verts, QList<int> &hors, QVector<QVector<QPair<int,QVector<SubInfo>>>>& schedObj);
    void drawSchedule(QPainter &painter,QRect pageRect, QString type, int row);
    void drawRotatedText(QPainter &painter, int x, int y, int width, int height, const QString &text);
    QFont fillRect(QString& str,QRect& rect, int flags, QFont& font);

private slots:

    void on_CatalogsChairsTable_cellDoubleClicked(int row, int column);
    void on_SubjectsSearchLine_textEdited(const QString &arg1);
    void on_ClassesSearchLine_textEdited(const QString &arg1);
    void on_RoomsSearchLine_textEdited(const QString &arg1);
    void on_TeachersSearchLine_textEdited(const QString &arg1);
    void on_ChairsSearchLine_textEdited(const QString &arg1);
    void on_ChairsSearchInCombo_currentIndexChanged(int index);
    void on_CatalogsClassesTable_cellDoubleClicked(int row, int column);
    void on_ExportAllClassesButton_clicked();
    void on_CatalogsTeachersTable_cellDoubleClicked(int row, int column);
};



#endif // CATALOGSFORM_H
