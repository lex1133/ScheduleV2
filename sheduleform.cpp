#include "sheduleform.h"
#include "ui_sheduleform.h"

SheduleForm::SheduleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SheduleForm)
{
    ui->setupUi(this);
}

SheduleForm::~SheduleForm()
{
    delete ui;
}
