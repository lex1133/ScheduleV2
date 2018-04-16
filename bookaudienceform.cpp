#include "bookaudienceform.h"
#include "ui_bookaudienceform.h"

BookAudienceForm::BookAudienceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookAudienceForm)
{
    ui->setupUi(this);
}

void BookAudienceForm::bookAudience(ScheduleForm* sform_,XMLParser *parser_, int week_, int day_, int hour_)
{
    this->sform = sform_;
    this->parser = parser_;
    this->week = week_;
    this->day = day_;
    this->hour = hour_;
    QDate beginDate = QDate::fromString(parser->getTerm()->beginDate,"dd.MM.yyyy");
    beginDate = beginDate.addDays(week*7);
    beginDate = beginDate.addDays(day);
    ui->dateLabel->setText(beginDate.toString("dd.MM.yyyy"));
    ui->timeLabel->setText(parser->getTimes()->time[hour+1]);
    QStringList wordList;
    for (QHash<int,Items::TeacherObj>::iterator i = parser->getTeachers()->begin(); i != parser->getTeachers()->end(); ++i)
    {
        wordList<<QString(i.value().surname + " " + i.value().firstName[0] + ". " + i.value().secondName[0] + ".");
    }

    QCompleter *completer = new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->teacherLine->setCompleter(completer);
}

void BookAudienceForm::editAudience(ScheduleForm* sform_,XMLParser *parser_, int week_, int day_, int hour_)
{
    this->sform = sform_;
    this->parser = parser_;
    this->week = week_;
    this->day = day_;
    this->hour = hour_;
    QDate beginDate = QDate::fromString(parser->getTerm()->beginDate,"dd.MM.yyyy");
    beginDate = beginDate.addDays(week*7);
    beginDate = beginDate.addDays(day);
    ui->dateLabel->setText(beginDate.toString("dd.MM.yyyy"));
    ui->timeLabel->setText(parser->getTimes()->time[hour+1]);
    QStringList wordList;
    for (QHash<int,Items::TeacherObj>::iterator i = parser->getTeachers()->begin(); i != parser->getTeachers()->end(); ++i)
    {
        wordList<<QString(i.value().surname + " " + i.value().firstName[0] + ". " + i.value().secondName[0] + ".");
    }

    QCompleter *completer = new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->teacherLine->setCompleter(completer);
    ui->teacherLine->setText(sform->reserved[week][day][hour].first);
    ui->reasonLine->setPlainText(sform->reserved[week][day][hour].second);

}

BookAudienceForm::~BookAudienceForm()
{
    delete ui;
}

void BookAudienceForm::on_OkPushButton_clicked()
{
    QPair<QString,QString> pair;
    pair.first = ui->teacherLine->text();
    pair.second = ui->reasonLine->toPlainText();
    sform->reserved[week][day][hour] = pair;
    sform->updateTable();
    this->close();
}

void BookAudienceForm::on_CancelPushButton_clicked()
{
    this->close();
}

void BookAudienceForm::on_DeletePushButton_clicked()
{
    sform->reserved[week][day][hour].first.clear();
    sform->reserved[week][day][hour].second.clear();
    sform->updateTable();
    this->close();
}
