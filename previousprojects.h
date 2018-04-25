#ifndef PREVIOUSPROJECTS_H
#define PREVIOUSPROJECTS_H

#include <QWidget>

namespace Ui {
class PreviousProjects;
}

class PreviousProjects : public QWidget
{
    Q_OBJECT

public:
    explicit PreviousProjects(QWidget *parent = 0);
    ~PreviousProjects();

private:
    Ui::PreviousProjects *ui;
};

#endif // PREVIOUSPROJECTS_H
