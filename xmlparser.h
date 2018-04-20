#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QFileInfo>
#include <QProgressDialog>


class XMLParser
{
public:
    XMLParser();
    bool ReadXMLData(QFile* file,QSqlDatabase* db_,QSqlQuery* query_);
    QString newbase;

private:

    bool GenerateBase();
    void ReadTerm();
    void ReadTimes();
    void ReadHolidays();
    void ReadClasses();
    void ReadSubjects();
    void ReadRooms();
    void ReadTeachers();
    void ReadStudyTypes();
    void ReadChairs();
    void ReadLoads();
    void ReadScheds();

    QSqlDatabase* db;
    QSqlQuery* query;
    QXmlStreamReader xml;    
    bool result = true;

};

#endif // XMLPARSER_H
