#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setEnabled(false);
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
            XMLParser parser;
            if(parser.ReadXMLData(file))
            {
                curProject = fileName;
                ui->centralWidget->setEnabled(true);
                ui->CatalogsTab->loadCatalogs(&parser);
                ui->ScheduleTab->loadShedule(&parser);
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

void MainWindow::on_MenuSaveProject_triggered()
{
    SaveProject(curProject);
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





