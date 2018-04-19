#ifndef BOOKAUDIENCEFORM_H
#define BOOKAUDIENCEFORM_H

#include "scheduleform.h"

#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QFile>
#include <QWidget>
#include <QDate>
#include <QCompleter>


class ScheduleForm;

namespace Ui {
class BookAudienceForm;
}

class BookAudienceForm : public QWidget
{
    Q_OBJECT

public:
    explicit BookAudienceForm(QWidget *parent = 0);
    void bookAudience(ScheduleForm* sform_,QSqlDatabase* db_,QSqlQuery* query_,int room_, int week_,int day_, int hour_);
    void editAudience(ScheduleForm* sform_,QSqlDatabase* db_,QSqlQuery* query_,int room_,int week_,int day_, int hour_);
    ~BookAudienceForm();

private slots:
    void on_OkPushButton_clicked();

    void on_CancelPushButton_clicked();

    void on_DeletePushButton_clicked();

private:
    QSqlDatabase* db;
    QSqlQuery* query;
    Ui::BookAudienceForm *ui;
    ScheduleForm* sform;
    int week;
    int day;
    int hour;
    int room;
};

#endif // BOOKAUDIENCEFORM_H
