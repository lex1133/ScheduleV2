#ifndef SHEDULEFORM_H
#define SHEDULEFORM_H

#include <QWidget>

namespace Ui {
class SheduleForm;
}

class SheduleForm : public QWidget
{
    Q_OBJECT

public:
    explicit SheduleForm(QWidget *parent = 0);
    ~SheduleForm();

private:
    Ui::SheduleForm *ui;
};

#endif // SHEDULEFORM_H
