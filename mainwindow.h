#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "xmlparser.h"
#include "catalogsform.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QtXml>
#include <QFile>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void OpenProject();
    bool SaveProject(QString fileName);

private slots:

    void on_MenuExitApp_triggered();

    void on_MenuOpenProject_triggered();

    void on_MenuSaveProject_triggered();

    void on_MenuImportFile_triggered();

    void on_MenuCloseProject_triggered();

private:
    Ui::MainWindow *ui;
    QString curProject;   
    XMLParser parser;
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // MAINWINDOW_H
