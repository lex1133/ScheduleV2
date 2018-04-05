#ifndef CATALOGSFORM_H
#define CATALOGSFORM_H

#include "items.h"
#include "xmlparser.h"

#include <QWidget>
#include <QDebug>

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

private slots:

    void on_CatalogsChairsTable_cellDoubleClicked(int row, int column);
    void on_SubjectsSearchLine_textEdited(const QString &arg1);
    void on_ClassesSearchLine_textEdited(const QString &arg1);
    void on_RoomsSearchLine_textEdited(const QString &arg1);
    void on_TeachersSearchLine_textEdited(const QString &arg1);
    void on_ChairsSearchLine_textEdited(const QString &arg1);
    void on_ChairsSearchInCombo_currentIndexChanged(int index);
};

#endif // CATALOGSFORM_H
