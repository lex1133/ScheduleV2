#include "catalogsform.h"
#include "ui_catalogsform.h"

CatalogsForm::CatalogsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CatalogsForm)
{
    ui->setupUi(this);
}

CatalogsForm::~CatalogsForm()
{
    delete ui;
}

void CatalogsForm::loadCatalogs(XMLParser *parser_)
{
    this->parser = parser_;
    loadChairs();
    loadTeachers();
    loadRooms();
    loadSubjects();
    loadClasses();
}

void CatalogsForm::loadChairs()
{
    ui->CatalogsChairsTable->setRowCount(parser->getChairs()->size());
    int j = 0;
    for (QHash<int,Items::ChairObj>::iterator i = parser->getChairs()->begin(); i != parser->getChairs()->end(); ++i)
    {
        ui->CatalogsChairsTable->setItem(j,0, new QTableWidgetItem(i.value().fullName));
        ui->CatalogsChairsTable->setItem(j,1, new QTableWidgetItem(i.value().shortName));
        j++;
    }
}

void CatalogsForm::loadTeachers()
{
    ui->CatalogsTeachersTable->setRowCount(parser->getTeachers()->size());
    int j = 0;
    for (QHash<int,Items::TeacherObj>::iterator i = parser->getTeachers()->begin(); i != parser->getTeachers()->end(); ++i)
    {
        ui->CatalogsTeachersTable->setItem(j,0, new QTableWidgetItem(i.value().surname + " " + i.value().firstName + ". " + i.value().secondName + "."));
        if(i.value().chairId != -1)
        {
            ui->CatalogsTeachersTable->setItem(j,1, new QTableWidgetItem(parser->getChairs()->value(i.value().chairId).fullName));
        }
        else
        {
            ui->CatalogsTeachersTable->setItem(j,1, new QTableWidgetItem("Не определена"));
        }

        j++;
    }
}

void CatalogsForm::loadRooms()
{
    ui->CatalogsRoomsTable->setRowCount(parser->getRooms()->size());
    int j = 0;
    for (QHash<int,Items::RoomObj>::iterator i = parser->getRooms()->begin(); i != parser->getRooms()->end(); ++i)
    {
        ui->CatalogsRoomsTable->setItem(j,0, new QTableWidgetItem(i.value().name));
        ui->CatalogsRoomsTable->setItem(j,1, new QTableWidgetItem(QString::number(i.value().capacity)));
        if(i.value().chairId != -1)
        {
            ui->CatalogsRoomsTable->setItem(j,2, new QTableWidgetItem(parser->getChairs()->value(i.value().chairId).fullName));
        }
        else
        {
            ui->CatalogsRoomsTable->setItem(j,2, new QTableWidgetItem("Не определена"));
        }

        j++;
    }
}

void CatalogsForm::loadSubjects()
{
    ui->CatalogsSubjectsTable->setRowCount(parser->getSubjects()->size());
    int j = 0;
    for (QHash<int,Items::SubjectObj>::iterator i = parser->getSubjects()->begin(); i != parser->getSubjects()->end(); ++i)
    {
        ui->CatalogsSubjectsTable->setItem(j,0, new QTableWidgetItem(i.value().fullName));
        ui->CatalogsSubjectsTable->setItem(j,1, new QTableWidgetItem(i.value().shortName));
        j++;
    }
}

void CatalogsForm::loadClasses()
{
    ui->CatalogsClassesTable->setRowCount(parser->getClasses()->size());
    int j = 0;
    for (QHash<int,Items::ClassObj>::iterator i = parser->getClasses()->begin(); i != parser->getClasses()->end(); ++i)
    {
        ui->CatalogsClassesTable->setItem(j,0, new QTableWidgetItem(i.value().name));
        ui->CatalogsClassesTable->setItem(j,1, new QTableWidgetItem(QString::number(i.value().students)));
        ui->CatalogsClassesTable->setItem(j,2, new QTableWidgetItem(QString::number(i.value().semester)));
        j++;
    }
}

void CatalogsForm::on_CatalogsChairsTable_cellDoubleClicked(int row, int column)
{

}

void CatalogsForm::on_SubjectsSearchLine_textEdited(const QString &arg1)
{
    auto search = ui->CatalogsSubjectsTable->findItems(arg1,Qt::MatchStartsWith);
    for( int i = 0; i < ui->CatalogsSubjectsTable->rowCount(); ++i )
    {
        bool match = false;

        QTableWidgetItem *item = ui->CatalogsSubjectsTable->item( i, 0 );
        if( search.contains(item))
        {
            match = true;
        }

        ui->CatalogsSubjectsTable->setRowHidden( i, !match );
    }
}

void CatalogsForm::on_ClassesSearchLine_textEdited(const QString &arg1)
{
    auto search = ui->CatalogsClassesTable->findItems(arg1,Qt::MatchStartsWith);
    for( int i = 0; i < ui->CatalogsClassesTable->rowCount(); ++i )
    {
        bool match = false;

        QTableWidgetItem *item = ui->CatalogsClassesTable->item( i, 0 );
        if( search.contains(item))
        {
            match = true;
        }

        ui->CatalogsClassesTable->setRowHidden( i, !match );
    }
}

void CatalogsForm::on_RoomsSearchLine_textEdited(const QString &arg1)
{
    auto search = ui->CatalogsRoomsTable->findItems(arg1,Qt::MatchStartsWith);
    for( int i = 0; i < ui->CatalogsRoomsTable->rowCount(); ++i )
    {
        bool match = false;

        QTableWidgetItem *item = ui->CatalogsRoomsTable->item( i, 0 );
        if( search.contains(item))
        {
            match = true;
        }

        ui->CatalogsRoomsTable->setRowHidden( i, !match );
    }
}

void CatalogsForm::on_TeachersSearchLine_textEdited(const QString &arg1)
{
    auto search = ui->CatalogsTeachersTable->findItems(arg1,Qt::MatchContains);
    for( int i = 0; i < ui->CatalogsTeachersTable->rowCount(); ++i )
    {
        bool match = false;

        QTableWidgetItem *item = ui->CatalogsTeachersTable->item( i, 0 );
        if( search.contains(item))
        {
            match = true;
        }

        ui->CatalogsTeachersTable->setRowHidden( i, !match );
    }
}

void CatalogsForm::on_ChairsSearchLine_textEdited(const QString &arg1)
{
    auto search = ui->CatalogsChairsTable->findItems(arg1,Qt::MatchContains);
    int column = ui->ChairsSearchInCombo->currentIndex();
    for( int i = 0; i < ui->CatalogsChairsTable->rowCount(); ++i )
    {
        bool match = false;

        QTableWidgetItem *item = ui->CatalogsChairsTable->item( i, column );
        if( search.contains(item))
        {
            match = true;
        }

        ui->CatalogsChairsTable->setRowHidden( i, !match );
    }
}

void CatalogsForm::on_ChairsSearchInCombo_currentIndexChanged(int index)
{
    on_ChairsSearchLine_textEdited((ui->ChairsSearchLine->text()));
}
