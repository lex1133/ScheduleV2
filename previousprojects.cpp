#include "previousprojects.h"
#include "ui_previousprojects.h"

PreviousProjects::PreviousProjects(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviousProjects)
{
    ui->setupUi(this);
}

PreviousProjects::~PreviousProjects()
{
    delete ui;
}
