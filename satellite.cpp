#include "satellite.h"
#include "ui_satellite.h"

Satellite::Satellite(QWidget *parent, const QString& name) :
    QDialog(parent),
    ui(new Ui::Satellite)
{
    ui->setupUi(this);

    ui->ok->setEnabled(false);
    ui->planet->setEnabled(false);

    QSqlQuery query;

    if (!query.exec("SELECT planet, length_of_equator, name "
                    "FROM satellites "
                    "WHERE name = '" + name + "';"))    {
        QMessageBox::critical(this, "Error!", "Unable to read data from satellite!");
        qDebug() << "Unable to read data from satellite table (constructor of Satellite class)";
    }

    query.first();
    QSqlRecord rec = query.record();

    _attr.length_of_equator = query.value(rec.indexOf("length_of_equator")).toString();
    _attr.name = query.value(rec.indexOf("name")).toString();
    _attr.planet = query.value(rec.indexOf("planet")).toString();


    ui->length_of_equator->setText(_attr.length_of_equator);
    ui->name->setText(_attr.name);
    ui->planet->setText(_attr.planet);

    setWindowTitle(_attr.name);
}

Satellite::~Satellite()
{
    delete ui;
}

void Satellite::on_cancel_clicked()
{
    close();
}

void Satellite::on_name_textChanged(const QString &arg1)
{
    ui->ok->setEnabled( !arg1.isEmpty() && !ui->length_of_equator->text().isEmpty() );
}

void Satellite::on_length_of_equator_textChanged(const QString &arg1)
{
    ui->ok->setEnabled( !arg1.isEmpty() && !ui->name->text().isEmpty() );
}

void Satellite::on_ok_clicked()
{
    QString str = "UPDATE satellites "
                  "SET length_of_equator = '%1', "
                  "name = '%2' "
                  "WHERE name = '" + _attr.name + "';",
            strF = str.arg(ui->length_of_equator->text())
            .arg(ui->name->text());
    QSqlQuery query;
    if (!query.exec(strF))  {
        qDebug() << "Unable to update data in satellites (on_ok_clicked() method of class Satellite";
        QMessageBox::critical(this, "Error!", "Unable to update data in table satellites!");
        close();
    }
    emit ok_clicked(_attr.name, ui->name->text());
    close();
}

void Satellite::on_Delete_clicked()
{
    QString str = "DELETE FROM satellites "
                  "WHERE name = '" + _attr.name + "';";
    QSqlQuery query;
    if (!query.exec(str))   {
        qDebug() << "Unable to delete data in satellites (on_delete_clicked() method of class Satellite";
        QMessageBox::critical(this, "Error!", "Unable to delete data in table satellites!");
        close();
    }
    str = "SELECT num_of_satellites "
          "FROM planets "
          "WHERE name = '" + _attr.planet + "';";

    if (!query.exec(str))   {
        qDebug() << "Unable to read data from planets table (on_delete_clicke() mathod of class  Satellite";
        QMessageBox::critical(this, "Error!", "Unable to read data from table planets!");
    }

    QSqlRecord rec = query.record();
    query.first();

    int num_of_satellites = query.value(rec.indexOf("num_of_satellites")).toInt();

    str = "UPDATE planets "
          "SET num_of_satellites = '" + QString::number(--num_of_satellites) + "' "
          "WHERE name = '" + _attr.planet + "'; ";

    if (!query.exec(str))   {
        qDebug() << "Unable to update data in planet table (on_delete_clicked() method of Satelite class";
        QMessageBox::critical(this, "Error!", "Unable to update data in planets!");
    }
    emit delete_clicked(_attr.name);
    close();
}
