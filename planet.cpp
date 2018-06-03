#include "planet.h"
#include "ui_planet.h"

Planet::Planet(QWidget *parent, const QString& name) :
    QDialog(parent),
    ui(new Ui::Planet)
{
    ui->setupUi(this);

    ui->ok->setEnabled(false);
    ui->num_of_satellites->setEnabled(false);


    QSqlQuery query;

    if (!query.exec("SELECT num_of_satellites, composition, length_of_equator, name "
                    "FROM planets "
                    "WHERE name = '" + name + "';"))    {
        QMessageBox::critical(this, "Error!", "Unable to read data from planet!");
        qDebug() << "Unable to read data from planet table (constructor of Planet class)";
    }

    query.first();
    QSqlRecord rec = query.record();

    _attr.length_of_equator = query.value(rec.indexOf("length_of_equator")).toString();
    _attr.composition = query.value(rec.indexOf("composition")).toString();
    _attr.num_of_satellites = query.value(rec.indexOf("num_of_satellites")).toString();
    _attr.name = query.value(rec.indexOf("name")).toString();

    ui->length_of_equator->setText(_attr.length_of_equator);
    ui->composition->setText(_attr.composition);
    ui->num_of_satellites->setText(_attr.num_of_satellites);
    ui->name->setText(_attr.name);

    setWindowTitle(_attr.name);
}

Planet::~Planet()
{
    delete ui;
}

void Planet::on_cancel_clicked()
{
    close();
}

void Planet::on_length_of_equator_textChanged(const QString &arg1)
{
    ui->ok->setEnabled( !arg1.isEmpty() && !ui->composition->text().isEmpty()
                        && !ui->name->text().isEmpty() );
}

void Planet::on_composition_textChanged(const QString &arg1)
{
    ui->ok->setEnabled( !arg1.isEmpty() && !ui->length_of_equator->text().isEmpty()
                        && !ui->name->text().isEmpty() );
}

void Planet::on_name_textChanged(const QString &arg1)
{
    ui->ok->setEnabled( !arg1.isEmpty() && !ui->composition->text().isEmpty()
                        && !ui->length_of_equator->text().isEmpty() );
}

void Planet::on_ok_clicked()
{
    QString str = "UPDATE planets "
                  "SET length_of_equator = '%1', "
                  "composition = '%2', "
                  "name = '%3' "
                  "WHERE name = '" + _attr.name + "';",
            strF = str.arg(ui->length_of_equator->text())
            .arg(ui->composition->text())
            .arg(ui->name->text());
    QSqlQuery query;
    if (!query.exec(strF))  {
        qDebug() << "Unable to update data in planets (on_ok_clicked() method of class Planet";
        QMessageBox::critical(this, "Error!", "Unable to update data in table!");
        close();
    }
    emit ok_clicked(_attr.name, ui->name->text());
    close();
}

void Planet::on_add_satellite_clicked()
{
    Add_satellite *satellite = new Add_satellite(this, _attr.name);
    satellite->show();
    connect(satellite, SIGNAL(add_clicked(QString, QString)), this, SLOT(on_add_clicked(QString, QString)));
}

void Planet::on_add_clicked(const QString &name, const QString &planet)
{
    QSqlQuery query;
    if (!query.exec("SELECT num_of_satellites "
                    "FROM planets "
                    "WHERE name = '" + _attr.name + "';"))  {
        qDebug() << "Unable to read data from planets, on_add_clicked() slot, Planet class";
        QMessageBox::critical(this, "Error!", "Unable to read data from planets!");
    }
    QSqlRecord rec = query.record();
    query.first();

    ui->num_of_satellites->setText(query.value(rec.indexOf("num_of_satellites")).toString());
    emit add_clicked(name, planet);
}

void Planet::on_Delete_clicked()
{
    QString str = "DELETE FROM satellites "
                  "WHERE planet = '" + _attr.name + "';";
    QSqlQuery query;
    if (!query.exec(str))   {
        qDebug() << "Unable to delete data from satellite, on_Delete_clicked() method in Planet class";
        QMessageBox::critical(this, "Error!", "Unable to delete data from satellite table");
        close();
    }

    str = "DELETE FROM planets "
          "WHERE name = '" + _attr.name + "';";
    if (!query.exec(str))   {
        qDebug() << "Unable to delete data from planet, on_Delete_cliked() method in Planet class";
        QMessageBox::critical(this, "Error!", "Unable to delete data from planet table");
        close();
    }
    str = "SELECT num_of_planet "
          "FROM star";

    if (!query.exec(str))   {
        qDebug() << "Unable to read data from star, on_Delete_cliked() slot in Planet class";
        QMessageBox::critical(this, "Error!", "Unable to read data from star!");
        close();
    }

    QSqlRecord rec = query.record();
    query.first();

    int num_of_planet = query.value(rec.indexOf("num_of_planet")).toInt();

    str = "UPDATE star "
          "SET num_of_planet = '" + QString::number(--num_of_planet) + "';";
    if (!query.exec(str))   {
        qDebug() << "Unable to update data from star, on_Delete_cliked() slot in Planet class";
        QMessageBox::critical(this, "Error!", "Unable to update data from star!");
        close();
    }
    emit delete_cliked(_attr.name);
    close();
}
