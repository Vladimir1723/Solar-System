#include "change.h"
#include "ui_change.h"

Change::Change(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Change)
{
    ui->setupUi(this);
    setWindowTitle("System");

    ui->ok->setEnabled(false);

    QRegExp reg_age("[0-9]{1}");
    ui->age->setValidator(new QRegExpValidator(reg_age, this));

    connect(ui->age, SIGNAL(textChanged(QString)), this, SLOT(on_age_textChanged(QString)));
    connect(ui->star_name, SIGNAL(textChanged(QString)), this, SLOT(on_star_name_textChanged(QString)));
    connect(ui->composition, SIGNAL(textChanged(QString)), this, SLOT(on_composition_textChanged(QString)));
}

Change::~Change()
{
    delete ui;
}

void Change::on_cancel_clicked()
{
    close();
}

void Change::init(QString age, QString star_name, QString composition)
{
    ui->age->setText(age);
    ui->star_name->setText(star_name);
    ui->composition->setText(composition);
}

void Change::on_star_name_textChanged(const QString& arg)
{
    ui->ok->setEnabled(!arg.isEmpty() && !ui->composition->text().isEmpty()
                           && !ui->age->text().isEmpty());
}

void Change::on_composition_textChanged(const QString& arg)
{
    ui->ok->setEnabled(!arg.isEmpty() && !ui->age->text().isEmpty() &&
                           !ui->star_name->text().isEmpty());
}

void Change::on_age_textChanged(const QString& arg)
{
    ui->ok->setEnabled(!arg.isEmpty() && !ui->composition->text().isEmpty()
                           && !ui->star_name->text().isEmpty());
}

void Change::on_ok_clicked()
{
    QSqlQuery query;

    QString strF = "UPDATE star "
                  "SET name = '%1', "
                  "age = '%2', "
                  "composition = '%3'";

    QString str = strF.arg(ui->star_name->text())
            .arg(ui->age->text())
            .arg(ui->composition->text());

    if (!query.exec(str)) {
        QMessageBox::critical(this, "Error!", "Cannot updating data in the database!");
        qDebug() << "Cannot updating data in the database (method on_ok_cliked(), class Change)";
        close();
    }

    emit this->ok_clicked();
    close();
}
