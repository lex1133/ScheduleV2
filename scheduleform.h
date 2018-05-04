#ifndef SCHEDULEFORM_H
#define SCHEDULEFORM_H


#include "bookaudienceform.h"

#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QWidget>
#include <QDebug>
#include <QDate>
#include <math.h>


namespace Ui {
class ScheduleForm;
}

class ScheduleForm : public QWidget
{
    Q_OBJECT

public:
    explicit ScheduleForm(QWidget *parent = 0);
    ~ScheduleForm();
    bool loadSchedule(QSqlDatabase* db_,QSqlQuery* query_);
    void updateTable();
    void closeSchedule();

private slots:

    void on_RoomsComboBox_currentIndexChanged(int index);

    void on_WeekComboBox_currentIndexChanged(int index);

    void on_Multimedia_stateChanged(int arg1);

    void on_capacity_editingFinished();

    void on_ScheduleTable_cellDoubleClicked(int row, int column);

    void on_RefreshButton_clicked();

    void on_ReservButton_clicked();

private:
    QDate beginDate;
    QDate endDate;
    Ui::ScheduleForm *ui;
    QSqlDatabase* db;
    QSqlQuery* query;
    bool loaded = false;
    bool loadResult = true;

    struct schedInfo
    {
        QString className;
        QString group;
        QString subjectName;
        QString teacherName;
        schedInfo() {}
    };
};

#endif // ScheduleFORM_H
