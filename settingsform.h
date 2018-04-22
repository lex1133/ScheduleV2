#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>
#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>

namespace Ui {
class SettingsForm;
}

class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = 0);
    ~SettingsForm();

private slots:
    void on_CancelButton_clicked();

    void on_SaveButton_clicked();

    void on_ResetSettingsButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::SettingsForm *ui;
    QSettings* sett;
    void saveSettings();
    void defaultSettings();
};

#endif // SETTINGSFORM_H
