#include "bookaudienceform.h"
#include "ui_bookaudienceform.h"

BookAudienceForm::BookAudienceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookAudienceForm)
{
    ui->setupUi(this);
}

void BookAudienceForm::bookAudience(ScheduleForm* sform_,QSqlDatabase* db_,QSqlQuery* query_, int room_,int week_, int day_, int hour_)
{
    this->sform = sform_;
    this->room = room_;
    this->week = week_;
    this->day = day_;
    this->hour = hour_;
    this->db = db_;
    this->query = query_;

    if(!query->exec("SELECT * FROM `Term`"))
    {
        qDebug()<<"Term query falure";
    }
    query->next();
    QDate beginDate = QDate::fromString(query->value(0).toString(),"dd.MM.yyyy");
    beginDate = beginDate.addDays(week*7);
    beginDate = beginDate.addDays(day);
    ui->dateLabel->setText(beginDate.toString("dd.MM.yyyy"));
    query->prepare("SELECT time FROM `Times` WHERE id=:timeId");
    query->bindValue(":timeId",hour+2);
    if(!query->exec())
    {
        qDebug()<<"Time query falure";
    }
    query->next();

    ui->timeLabel->setText(query->value(0).toString());
    QStringList wordList;
    if(!query->exec("SELECT * FROM `Teachers`"))
    {
        qDebug()<<"Teachers query falure";
    }
    while(query->next())
    {
        wordList<<QString(query->value(1).toString() + " " + query->value(2).toString()[0] + ". " + query->value(3).toString()[0] + ".");
    }

    QCompleter *completer = new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->teacherLine->setCompleter(completer);

    QStringList wordList2;
    if(!query->exec("SELECT * FROM `Classes`"))
    {
        qDebug()<<"Classes query falure";
    }
    while(query->next())
    {
        wordList2<<query->value(1).toString();
    }

    QCompleter *completer2 = new QCompleter(wordList2, this);
    completer2->setCaseSensitivity(Qt::CaseInsensitive);
    ui->classLine->setCompleter(completer2);

    query->finish();
}

void BookAudienceForm::editAudience(ScheduleForm* sform_,QSqlDatabase* db_,QSqlQuery* query_, int room_, int week_, int day_, int hour_)
{
    this->sform = sform_;
    this->room = room_;
    this->week = week_;
    this->day = day_;
    this->hour = hour_;
    this->db = db_;
    this->query = query_;

    if(!query->exec("SELECT * FROM `Term`"))
    {
        qDebug()<<"Term query falure";
    }
    query->next();
    QDate beginDate = QDate::fromString(query->value(0).toString(),"dd.MM.yyyy");
    beginDate = beginDate.addDays(week*7);
    beginDate = beginDate.addDays(day);
    ui->dateLabel->setText(beginDate.toString("dd.MM.yyyy"));
    query->prepare("SELECT time FROM `Times` WHERE id=:timeId");
    query->bindValue(":timeId",hour+2);
    if(!query->exec())
    {
        qDebug()<<"Time query falure";
    }
    query->next();

    ui->timeLabel->setText(query->value(0).toString());
    QStringList wordList;
    if(!query->exec("SELECT * FROM `Teachers`"))
    {
        qDebug()<<"Teachers query falure";
    }
    while(query->next())
    {
        wordList<<QString(query->value(1).toString() + " " + query->value(2).toString()[0] + ". " + query->value(3).toString()[0] + ".");
    }

    QCompleter *completer = new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->teacherLine->setCompleter(completer);

    QStringList wordList2;
    if(!query->exec("SELECT * FROM `Classes`"))
    {
        qDebug()<<"Classes query falure";
    }
    while(query->next())
    {
        wordList2<<query->value(1).toString();
    }

    QCompleter *completer2 = new QCompleter(wordList2, this);
    completer2->setCaseSensitivity(Qt::CaseInsensitive);
    ui->classLine->setCompleter(completer2);

    query->prepare("SELECT * FROM `Reserved` WHERE room=:room AND week=:week AND day=:day AND hour=:hour");
    query->bindValue(":room",room);
    query->bindValue(":week",week);
    query->bindValue(":day",day);
    query->bindValue(":hour",hour);
    if(!query->exec())
    {
        qDebug()<<"Reserver query falure";
    }
    query->next();
    ui->teacherLine->setText(query->value(5).toString());    
    ui->classLine->setText(query->value(6).toString());
    ui->reasonLine->setPlainText(query->value(7).toString());
    query->finish();
}

BookAudienceForm::~BookAudienceForm()
{
    delete ui;
}

void BookAudienceForm::on_OkPushButton_clicked()
{
    query->prepare("INSERT INTO `Reserved`(`room`,`week`,`day`,`hour`,`teacher`,`class`,`reason`) VALUES (:room,:week,:day,:hour,:teacher,:class,:reason)");
    query->bindValue(":room",room);
    query->bindValue(":week",week);
    query->bindValue(":day",day);
    query->bindValue(":hour",hour);
    query->bindValue(":teacher",ui->teacherLine->text());
    query->bindValue(":class",ui->classLine->text());
    query->bindValue(":reason",ui->reasonLine->toPlainText());
    if(!query->exec())
    {
        qDebug()<<"Reserver query falure";
        qDebug()<<query->lastError();
    }
    query->finish();
    sform->updateTable();
    this->close();
}

void BookAudienceForm::on_CancelPushButton_clicked()
{
    this->close();
}

void BookAudienceForm::on_DeletePushButton_clicked()
{
    query->prepare("DELETE FROM `Reserved` WHERE room=:room AND week=:week AND day=:day AND hour=:hour");
    query->bindValue(":room",room);
    query->bindValue(":week",week);
    query->bindValue(":day",day);
    query->bindValue(":hour",hour);
    if(!query->exec())
    {
        qDebug()<<"Reserver query falure";
    }

    query->finish();
    sform->updateTable();
    this->close();
}
