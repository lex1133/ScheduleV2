#ifndef CATALOGSFORM_H
#define CATALOGSFORM_H

#include "items.h"
#include "xmlparser.h"
#include "chairchangedialog.h"

#include <QWidget>
#include <QMessageBox>

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

private slots:

    void on_CatalogsChairsTable_cellDoubleClicked(int row, int column);

    void on_AddChairButton_clicked();

    void on_DeleteChairButton_clicked();

};

#endif // CATALOGSFORM_H
