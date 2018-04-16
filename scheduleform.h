#ifndef SCHEDULEFORM_H
#define SCHEDULEFORM_H

#include "xmlparser.h"
#include "items.h"
#include "bookaudienceform.h"

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
    void loadSchedule(XMLParser* parser_);
    void updateTable();
    QVector<QVector<QVector<QPair<QString,QString>>>> reserved;

private slots:

    void on_RoomsComboBox_currentIndexChanged(int index);

    void on_WeekComboBox_currentIndexChanged(int index);

    void on_Multimedia_stateChanged(int arg1);

    void on_capacity_editingFinished();

    void on_ScheduleTable_cellDoubleClicked(int row, int column);

private:
    QDate beginDate;
    QDate endDate;
    Ui::ScheduleForm *ui;
    XMLParser* parser;
    bool loaded = false;

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
