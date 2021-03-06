#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setEnabled(false);
    //PreviousProjects* prp = new PreviousProjects(0);
    //prp->show();
    QFileInfo fin(QApplication::arguments()[0]);
    QSettings* sett = new QSettings(fin.absolutePath() + "\\settings.ini",QSettings::IniFormat);
    if(!sett->contains("PathToSave"))
    {
        QMessageBox::information(0,tr("Конфигурация не определнна"), tr("Файл конфигурации не найден или поврежден!<br>"
                                                                        "Пожалуйста произведите настройку программы."),QMessageBox::Ok);
        SettingsForm* sf = new SettingsForm(0);
        sf->show();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::OpenProject()
{

    QFileInfo fin(QApplication::arguments()[0]);
    QSettings* sett = new QSettings(fin.absolutePath() + "\\settings.ini",QSettings::IniFormat);

    auto fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Открыть расписание"), sett->value("PrevPath").toString() , tr("Файл расписания (*.xml)"));
    if(!fileName.isEmpty())
    {
        QString check = fileName.mid(0,fileName.length()-4) + ".scdb";
        if(QFileInfo(check).exists())
        {
            fileName = check;
        }
        QFile argFile(fileName);
        QFileInfo argFin(argFile);

        if(argFin.fileName().mid(argFin.fileName().length()-5,5) == ".scdb")
        {
            if(curProject != "" && curProject != fileName)
            {
                auto answer = QMessageBox::warning(this,tr("Подтверждение закрытия"),tr("Закрыть?"),
                                                   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel , QMessageBox::Cancel);
                if(answer == QMessageBox::No)
                {
                    return;
                }
                else if(answer == QMessageBox::Cancel)
                {
                    return;
                }
            }

            QFile* file = new QFile(argFin.absoluteFilePath());
            if (!file->open(QIODevice::ReadWrite | QIODevice::Text))
            {
                QMessageBox::critical(this,tr("Ошибка"),tr("Невозможно открыть базу данных"), QMessageBox::Ok);
            }
            else
            {
                curProject = argFin.fileName().mid(0,argFin.fileName().length()-5);
                sett->setValue("PrevPath",argFin.absolutePath());
                QTime t;
                t.start();

                ui->CatalogsTab->closeCatalogs();
                ui->ScheduleTab->closeSchedule();
                QFileInfo fin(*file);

                db = QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName(fin.absoluteFilePath());

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
                if(ui->CatalogsTab->loadCatalogs(this,&db,&query) && ui->ScheduleTab->loadSchedule(&db,&query))
                {
                    ui->centralWidget->setEnabled(true);
                }
                else
                {
                    ui->CatalogsTab->closeCatalogs();
                    ui->ScheduleTab->closeSchedule();
                    QMessageBox::critical(this,tr("Ошибка"),tr("Невозможно загрузить базу данных"), QMessageBox::Ok);
                }

                qDebug("Time elapsed: %d ms", t.elapsed());
            }
        }
        else if(argFin.fileName().mid(argFin.fileName().length()-4,4) == ".xml")
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

                ui->CatalogsTab->closeCatalogs();
                ui->ScheduleTab->closeSchedule();
                ui->statusBar->showMessage("Идет загрузка XML...");
                parser.ReadXMLData(file,&db,&query);
                ui->statusBar->showMessage("Загрузка XML прошла успешно!",1500);
                if(curProject != "" && curProject != db.databaseName())
                {
                    auto answer = QMessageBox::warning(this,tr("Подтверждение закрытия"),tr("Закрыть?"),
                                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel , QMessageBox::Cancel);
                    if(answer == QMessageBox::No)
                    {
                        return;
                    }
                    else if(answer == QMessageBox::Cancel)
                    {
                        return;
                    }
                }
                query = QSqlQuery(db);

                curProject = argFin.fileName().mid(0,argFin.fileName().length()-4);
                sett->setValue("PrevPath",argFin.absolutePath());
                if(ui->CatalogsTab->loadCatalogs(this,&db,&query) && ui->ScheduleTab->loadSchedule(&db,&query))
                {
                    ui->centralWidget->setEnabled(true);
                }
                else
                {
                    ui->CatalogsTab->closeCatalogs();
                    ui->ScheduleTab->closeSchedule();
                    QMessageBox::critical(this,tr("Ошибка"),tr("Невозможно загрузить базу данных"), QMessageBox::Ok);
                }
                qDebug("Time elapsed: %d ms", t.elapsed());
            }
        }

    }
}

bool MainWindow::SaveProject(QString fileName)
{
    return true;
}


void MainWindow::on_MenuOpenProject_triggered()
{
    OpenProject();
}

void MainWindow::on_MenuExitApp_triggered()
{
    auto answer = QMessageBox::warning(this,tr("Подтверждение выхода"),tr("Выйти?"),
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel , QMessageBox::Cancel);
    if(answer == QMessageBox::Yes)
    {
        QApplication::exit(0);
    }

}

void MainWindow::on_MenuCloseProject_triggered()
{
    auto answer = QMessageBox::warning(this,tr("Подтверждение закрытия"),tr("Закрыть?"),
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel , QMessageBox::Cancel);
    if(answer == QMessageBox::Yes)
    {
        ui->CatalogsTab->closeCatalogs();
        ui->ScheduleTab->closeSchedule();
        ui->tabWidget->setCurrentIndex(0);
        ui->centralWidget->setEnabled(false);
    }
}

void MainWindow::on_MenuAbout_triggered()
{
    QMessageBox::about(this,tr("О программе"),tr("<center><h1>Расписание V2.0</h1></center><br>"
                                                 "Программа разработанна по заказу МГТУ ВО \"СТАНКИН\" в рамках производственной практики<br>"
                                                 "Автор: Данилин Алексей Олегович ИДБ-15-15<br>"
                                                 "E-mail: <a href=\"mailto:stud115089@stankin.ru\" target=\"_top\">stud115089@stankin.ru</a><br>"
                                                 "Ссылка на исходники: <a href=\"https://github.com/lex1133/ScheduleV2\">https://github.com/lex1133/ScheduleV2</a>"));
}

void MainWindow::on_MenuSettings_triggered()
{
    SettingsForm* sf = new SettingsForm(0);
    sf->show();
}

void MainWindow::on_MenuHelp_triggered()
{
    InfoWidget* iw = new InfoWidget();
    iw->show();
}

void MainWindow::on_MenuDownloadLatest_triggered()
{

    QDesktopServices::openUrl(QUrl("https://github.com/lex1133/ScheduleV2/releases"));
}
