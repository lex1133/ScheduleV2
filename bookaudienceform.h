#ifndef BOOKAUDIENCEFORM_H
#define BOOKAUDIENCEFORM_H

#include "items.h"
#include "xmlparser.h"
#include "scheduleform.h"

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
    void bookAudience(ScheduleForm* sform_,XMLParser* parser_,int week_,int day_, int hour_);
    void editAudience(ScheduleForm* sform_,XMLParser* parser_,int week_,int day_, int hour_);
    ~BookAudienceForm();

private slots:
    void on_OkPushButton_clicked();

    void on_CancelPushButton_clicked();

    void on_DeletePushButton_clicked();

private:
    Ui::BookAudienceForm *ui;
    int week;
    int day;
    int hour;
    ScheduleForm* sform;
    XMLParser* parser;
};

#endif // BOOKAUDIENCEFORM_H
