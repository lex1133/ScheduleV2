#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
    QFileInfo fin(QApplication::arguments()[0]);
    sett = new QSettings(fin.absolutePath() + "\\settings.ini",QSettings::IniFormat);
    if(!sett->contains("PathToSave"))
    {
        QFile::remove(fin.absolutePath() + "\\settings.ini");
        defaultSettings();
    }
    ui->PathToSave->setText(sett->value("PathToSave").toString());
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::on_CancelButton_clicked()
{
    this->close();
}

void SettingsForm::on_SaveButton_clicked()
{
    saveSettings();
    this->close();
}

void SettingsForm::saveSettings()
{    
    sett->setValue("PathToSave",ui->PathToSave->text());
}

void SettingsForm::defaultSettings()
{

    QFileInfo fin(QApplication::arguments()[0]);
    QDir dir(fin.absolutePath());
    dir.cdUp();
    sett->setValue("PathToSave", dir.absolutePath());
}

void SettingsForm::on_ResetSettingsButton_clicked()
{
    defaultSettings();
    ui->PathToSave->setText(sett->value("PathToSave").toString());
}

void SettingsForm::on_pushButton_clicked()
{
    QFileDialog* d;
    QString path = d->getExistingDirectory(0,tr("Выбрать путь сохранения"),sett->value("PathToSave").toString());
    if(!path.isEmpty())
    {
        sett->setValue("PathToSave",path);
        ui->PathToSave->setText(path);
    }

}
