#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setEnabled(false);
    if(QApplication::arguments().size() > 1)
    {
        QFile argFile(QApplication::arguments()[1]);
        QFileInfo argFin(argFile);
        if(argFin.completeSuffix() == "scdb")
        {
            QFile* file = new QFile(argFin.absoluteFilePath());
            if (!file->open(QIODevice::ReadWrite | QIODevice::Text))
            {
                QMessageBox::critical(this,tr("Ошибка"),tr("Невозможно открыть базу данных"), QMessageBox::Ok);
            }
            else
            {
                QTime t;
                t.start();
                QFileInfo fin(*file);
                db = QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName(fin.absoluteFilePath());
                db.open();
                if (fin.isFile()) {
                    if (db.open()) {
                        qDebug()<<"[+] Connected to Database File";
                    }
                    else {
                        qDebug()<<"[!] Database File was not opened";
                    }
                }
                else {
                    qDebug()<<"[!] Database File does not exist";
                }
                query = QSqlQuery(db);
                if(ui->CatalogsTab->loadCatalogs(&db,&query) && ui->ScheduleTab->loadSchedule(&db,&query))
                {
                    ui->centralWidget->setEnabled(true);
                }
                else
                {
                    QMessageBox::critical(this,tr("Ошибка"),tr("Невозможно загрузить базу данных"), QMessageBox::Ok);
                }

                qDebug("Time elapsed: %d ms", t.elapsed());
            }
        }
        else if(argFin.completeSuffix() == "xml")
        {
            QFile* file = new QFile(argFin.absoluteFilePath());
            if (!file->open(QIODevice::ReadWrite | QIODevice::Text))
            {
                QMessageBox::critical(this,tr("Ошибка"),tr("Невозможно открыть XML-конфиг"), QMessageBox::Ok);
            }
            else
            {

                QTime t;
                t.start();
                ui->statusBar->showMessage("Идет загрузка XML...");
                parser.ReadXMLData(file,&db,&query);
                ui->statusBar->showMessage("Загрузка XML прошла успешно!",1500);
                query = QSqlQuery(db);
                if(ui->CatalogsTab->loadCatalogs(&db,&query) && ui->ScheduleTab->loadSchedule(&db,&query))
                {
                    ui->centralWidget->setEnabled(true);
                }
                else
                {
                    QMessageBox::critical(this,tr("Ошибка"),tr("Невозможно загрузить базу данных"), QMessageBox::Ok);
                }
                qDebug("Time elapsed: %d ms", t.elapsed());
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::OpenProject()
{
    auto fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Открыть расписание"), "" , tr("SCDB файл (*.scdb)"));
    if(!fileName.isEmpty())
    {
        QFile* file = new QFile(fileName);
        if (!file->open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QMessageBox::critical(this,tr("Ошибка"),tr("Невозможно открыть базу данных"), QMessageBox::Ok);
        }
        else
        {
            QTime t;
            t.start();
            QFileInfo fin(*file);
            db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(fin.absoluteFilePath());
            db.open();
            if (fin.isFile()) {
                if (db.open()) {
                    qDebug()<<"[+] Connected to Database File";
                }
                else {
                    qDebug()<<"[!] Database File was not opened";
                }
            }
            else {
                qDebug()<<"[!] Database File does not exist";
            }
            query = QSqlQuery(db);
            if(ui->CatalogsTab->loadCatalogs(&db,&query) && ui->ScheduleTab->loadSchedule(&db,&query))
            {
                ui->centralWidget->setEnabled(true);
            }
            else
            {
                QMessageBox::critical(this,tr("Ошибка"),tr("Невозможно загрузить базу данных"), QMessageBox::Ok);
            }

            qDebug("Time elapsed: %d ms", t.elapsed());
        }
    }
}

bool MainWindow::SaveProject(QString fileName)
{

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






void MainWindow::on_MenuImportFile_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Открыть расписание"), "" , tr("XML файл (*.xml)"));
    if(!fileName.isEmpty())
    {
        QFile* file = new QFile(fileName);
        if (!file->open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QMessageBox::critical(this,tr("Ошибка"),tr("Невозможно открыть XML-конфиг"), QMessageBox::Ok);
        }
        else
        {

            QTime t;
            t.start();
            ui->statusBar->showMessage("Идет загрузка XML...");
            parser.ReadXMLData(file,&db,&query);
            ui->statusBar->showMessage("Загрузка XML прошла успешно!",1500);
            query = QSqlQuery(db);
            if(ui->CatalogsTab->loadCatalogs(&db,&query) && ui->ScheduleTab->loadSchedule(&db,&query))
            {
                ui->centralWidget->setEnabled(true);
            }
            else
            {
                QMessageBox::critical(this,tr("Ошибка"),tr("Невозможно загрузить базу данных"), QMessageBox::Ok);
            }
            qDebug("Time elapsed: %d ms", t.elapsed());
        }
    }
}

void MainWindow::on_MenuCloseProject_triggered()
{
    auto answer = QMessageBox::warning(this,tr("Подтверждение закрытия"),tr("Сохранить текущее расписание?"),
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel , QMessageBox::Cancel);
    if(answer == QMessageBox::Yes)
    {
        if(SaveProject(curProject))
        {
            ui->CatalogsTab->closeCatalogs();
            ui->ScheduleTab->closeSchedule();
            ui->tabWidget->setCurrentIndex(0);
            ui->centralWidget->setEnabled(false);
        }
    }
    else if(answer == QMessageBox::No)
    {
        ui->CatalogsTab->closeCatalogs();
        ui->ScheduleTab->closeSchedule();
        ui->tabWidget->setCurrentIndex(0);
        ui->centralWidget->setEnabled(false);
    }
}
