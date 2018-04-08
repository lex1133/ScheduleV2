#ifndef CATALOGSFORM_H
#define CATALOGSFORM_H

#include "items.h"
#include "xmlparser.h"

#include <QWidget>
#include <QDebug>
#include <QPrinter>
#include <QPainter>
#include <QDate>

namespace Ui {
class CatalogsForm;
}

class CatalogsForm : public QWidget
{
    Q_OBJECT

public:
    explicit CatalogsForm(QWidget *parent = 0);
    ~CatalogsForm();
    void loadCatalogs(XMLParser* parser_);

private:
    Ui::CatalogsForm *ui;    
    XMLParser* parser;
    void loadChairs();
    void loadTeachers();
    void loadRooms();
    void loadSubjects();
    void loadClasses();
    void drawClassSched(QPainter &painter, QList<int> &verts, QList<int> &hors, QString className);
    void drawSchedule(QPainter &painter,QRect pageRect, QString type, int row);
    void drawRotatedText(QPainter &painter, int x, int y, int width, int height, const QString &text);
    static bool compare(const QPair<QDate,QDate> &x, const QPair<QDate,QDate> &y)
    {
        return x.first < y.first;
    }
private slots:

    void on_CatalogsChairsTable_cellDoubleClicked(int row, int column);
    void on_SubjectsSearchLine_textEdited(const QString &arg1);
    void on_ClassesSearchLine_textEdited(const QString &arg1);
    void on_RoomsSearchLine_textEdited(const QString &arg1);
    void on_TeachersSearchLine_textEdited(const QString &arg1);
    void on_ChairsSearchLine_textEdited(const QString &arg1);
    void on_ChairsSearchInCombo_currentIndexChanged(int index);
    void on_CatalogsClassesTable_cellDoubleClicked(int row, int column);
};

#endif // CATALOGSFORM_H
