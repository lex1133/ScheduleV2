#ifndef CHAIRCHANGEDIALOG_H
#define CHAIRCHANGEDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QMessageBox>

namespace Ui {
class ChairChangeDialog;
}

class ChairChangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChairChangeDialog(QWidget *parent = 0);
    void setItem(QTableWidgetItem* fNameitem_,QTableWidgetItem* sNameitem_);
    ~ChairChangeDialog();

private slots:
    void on_OKButton_clicked();

    void on_CancelButton_clicked();

private:
    Ui::ChairChangeDialog *ui;
    QTableWidgetItem* fNameitem;
    QTableWidgetItem* sNameitem;
};

#endif // CHAIRCHANGEDIALOG_H
