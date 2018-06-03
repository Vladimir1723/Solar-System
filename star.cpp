#include "star.h"
#include "ui_star.h"

Star::Star(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Star)
{
    ui->setupUi(this);

    QRegExp exp_age("[0-9]{1}"), exp_temp("[1-9]{1}[0-9]{0,6}");
    ui->age_value->setValidator(new QRegExpValidator(exp_age, this));
    ui->temperature_value->setValidator(new QRegExpValidator(exp_temp, this));

    ui->ok->setEnabled(false);

    QSqlQuery query;

    if (!query.exec("SELECT name, age, composition "
                    "FROM star")) {
        qDebug() << "Cannot read data from the database (star), (in constructor of class Star)";
        QMessageBox::critical(this, "Error!", "Cannot reading data from the database (star)!");
    }

    QSqlRecord rec = query.record();
    query.first();

    _attr.name = query.value(rec.indexOf("name")).toString();
    _attr.age = query.value(rec.indexOf("age")).toString();
    _attr.composition = query.value(rec.indexOf("composition")).toString();

    QString strF = "UPDATE sun "
                  "SET name = '%1', "
                  "age = %2, "
                  "composition = '%3'";

    QString str = strF.arg(_attr.name)
            .arg(_attr.age)
            .arg(_attr.composition);

    query.clear();

    if (!query.exec(str))   {
        qDebug() << "Cannot updating data from the database (sun)!";
    }

    if (!query.exec("SELECT name, age, composition, temperature "
                    "FROM sun"))    {
        qDebug() << "Cannot read data from the database (sun), (in constructor of class Star)";
        QMessageBox::critical(this, "Error!", "Cannot reading data from the database (sun)!");
    }

    rec = query.record();
    query.first();

    _attr.name = query.value(rec.indexOf("name")).toString();
    _attr.age = query.value(rec.indexOf("age")).toString();
    _attr.composition = query.value(rec.indexOf("composition")).toString();
    _attr.temperature = query.value(rec.indexOf("temperature")).toString();

    ui->age_value->setText(_attr.age);
    ui->composition_value->setText(_attr.composition);
    ui->temperature_value->setText(_attr.temperature);

    _pix.load(":/watercolor-sun-1.png");
    _pix = _pix.scaled(100, 100, Qt::KeepAspectRatio);

    ui->picture->resize(_pix.size());
    ui->picture->setPixmap(_pix);

    setWindowTitle(_attr.name);
}

Star::~Star()
{
    delete ui;
}

void Star::on_close_clicked()
{
    close();
}

void Star::on_ok_clicked()
{
    QSqlQuery query;

    QString strF = "UPDATE sun "
                  "SET age = '%1', "
                  "composition = '%2', "
                  "temperature = '%3'";

    QString str = strF.arg(ui->age_value->text())
            .arg(ui->composition_value->text())
            .arg(ui->temperature_value->text());

    if (!query.exec(str)) {
        QMessageBox::critical(this, "Error!", "Cannot updating data in the database (sun)");
        qDebug() << "Cannot updating data in the database (method on_ok_cliked(), class Star)";
        close();
    }

    emit this->ok_clicked();
    close();
}

void Star::on_age_value_textChanged(const QString &arg1)
{
    ui->ok->setEnabled( !arg1.isEmpty() && !ui->composition_value->text().isEmpty()
                        && !ui->temperature_value->text().isEmpty() );
}

void Star::on_composition_value_textChanged(const QString &arg1)
{
    ui->ok->setEnabled( !arg1.isEmpty() && !ui->age_value->text().isEmpty()
                        && !ui->temperature_value->text().isEmpty() );
}

void Star::on_temperature_value_textChanged(const QString &arg1)
{
    ui->ok->setEnabled( !arg1.isEmpty() && !ui->composition_value->text().isEmpty()
                        && !ui->age_value->text().isEmpty() );
}

void Star::on_add_planet_clicked()
{
    Add_planet *planet = new Add_planet(this);
    planet->show();
    connect(planet, SIGNAL(add_clicked(QString)), this, SIGNAL(add_clicked(QString)));
}
