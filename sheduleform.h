#ifndef SHEDULEFORM_H
#define SHEDULEFORM_H

#include "xmlparser.h"
#include "items.h"

#include <QWidget>
#include <QDebug>
#include <QDate>
#include <math.h>


namespace Ui {
class SheduleForm;
}

class SheduleForm : public QWidget
{
    Q_OBJECT

public:
    explicit SheduleForm(QWidget *parent = 0);
    ~SheduleForm();
    void loadShedule(XMLParser* parser_);
    void updateTable();

private slots:

    void on_RoomsComboBox_currentIndexChanged(int index);

    void on_WeekComboBox_currentIndexChanged(int index);

private:
    QDate beginDate;
    QDate endDate;
    Ui::SheduleForm *ui;
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

#endif // SHEDULEFORM_H
