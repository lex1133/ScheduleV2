#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::OpenProject()
{
    auto fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Открыть расписание"), tr("C:\\Users") , tr("XML файл (*.xml)"));
    if(!fileName.isEmpty())
    {
        QFile* file = new QFile(fileName);
        if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(this,tr("Ошибка"),tr("Невозможно открыть XML-конфиг"), QMessageBox::Ok);
        }
        else
        {
            curProject = fileName;
            XMLParser parser;
            parser.ReadXMLData(file);
            ui->CatalogsChairsTable->setRowCount(parser.getChairs()->size());
            int j = 0;
            for (QHash<int,Items::ChairObj>::iterator i = parser.getChairs()->begin(); i != parser.getChairs()->end(); ++i)
            {
                ui->CatalogsChairsTable->setItem(j,0, new QTableWidgetItem(i.value().fullName));
                ui->CatalogsChairsTable->setItem(j,1, new QTableWidgetItem(i.value().shortName));
                j++;
            }
        }
    }
}

bool MainWindow::SaveProject(QString fileName)
{
    if(fileName.isEmpty())
        fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Сохранить расписание"), tr("C:\\Users"), tr("XML файл (*.xml)"));
    if(!fileName.isEmpty())
    {
        return true;
    }
    else
    {
        QMessageBox::critical(this,tr("Ошибка"),tr("Файл сохраниения не указан!"), QMessageBox::Ok);
        return false;
    }
}


void MainWindow::on_MenuOpenProject_triggered()
{
    OpenProject();
}

void MainWindow::on_MenuExitApp_triggered()
{
    auto answer = QMessageBox::warning(this,tr("Подтверждение выхода"),tr("Сохранить текущее расписание?"),
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel , QMessageBox::Cancel);
    if(answer == QMessageBox::Yes)
    {
        if(SaveProject(curProject))
        {
            QApplication::exit(0);
        }
    }
    else if(answer == QMessageBox::No)
    {
        QApplication::exit(0);
    }
}



void MainWindow::on_MenuSaveProject_triggered()
{
    SaveProject(curProject);
}

void MainWindow::on_CatalogsChairsTable_cellDoubleClicked(int row, int column)
{
    ChairChangeDialog* chd = new ChairChangeDialog;
    chd->setItem(ui->CatalogsChairsTable->item(row,0),ui->CatalogsChairsTable->item(row,1));
    chd->show();
}
