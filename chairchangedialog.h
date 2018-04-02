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
    void editItem(QTableWidget* table_, int row_);
    ~ChairChangeDialog();

private slots:
    void on_OKButton_clicked();

    void on_CancelButton_clicked();

private:
    Ui::ChairChangeDialog *ui;
    QTableWidget* table;
    int row;
};

#endif // CHAIRCHANGEDIALOG_H
