#ifndef SHEDULEFORM_H
#define SHEDULEFORM_H

#include "xmlparser.h"
#include "items.h"

#include <QWidget>
#include <QDebug>
#include <QDate>
#include <math.h>

namespace Ui {
class SheduleForm;
}

class SheduleForm : public QWidget
{
    Q_OBJECT

public:
    explicit SheduleForm(QWidget *parent = 0);
    ~SheduleForm();
    void loadShedule(XMLParser* parser_);
    void updateTable();

private:
    Ui::SheduleForm *ui;
    XMLParser* parser;
};

#endif // SHEDULEFORM_H
