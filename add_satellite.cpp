#include "add_satellite.h"
#include "ui_add_satellite.h"

Add_satellite::Add_satellite(QWidget *parent, const QString& name_of_planet) :
    QDialog(parent),
    ui(new Ui::Add_satellite)
{
    ui->setupUi(this);

    ui->add->setEnabled(false);
    ui->planet->setEnabled(false);
    ui->planet->setText(name_of_planet);

    setWindowTitle("Add satellite");
}

Add_satellite::~Add_satellite()
{
    delete ui;
}

void Add_satellite::on_cancel_clicked()
{
    close();
}

void Add_satellite::on_name_textChanged(const QString &arg1)
{
    ui->add->setEnabled( !arg1.isEmpty() && !ui->length_of_equator->text().isEmpty() );
}

void Add_satellite::on_length_of_equator_textChanged(const QString &arg1)
{
    ui->add->setEnabled( !arg1.isEmpty() && !ui->name->text().isEmpty() );
}

void Add_satellite::on_add_clicked()
{
    QSqlQuery query;

    QString str = "INSERT INTO satellites "
                  "VALUES ('%1', '%2', '%3');",
            strF = str.arg(ui->name->text(), ui->planet->text(), ui->length_of_equator->text());

    if (!query.exec(strF))  {
        qDebug() << "Unable to insert data into satellites, on_add_clicked() method of Add_satellite class";
        QMessageBox::critical(this, "Error!", "Unable to insert data into satellites!");
        close();
    }

    strF = "SELECT num_of_satellites "
           "FROM planets "
           "WHERE name = '" + ui->planet->text() + "';";

    if (!query.exec(strF))  {
        qDebug() << "Unable to read data from planets, on_add_clicked() method of Add_satellite class";
        QMessageBox::critical(this, "Error!", "Unable to read data from planets!");
        close();
    }

    QSqlRecord rec = query.record();
    query.first();

    int num_of_satellites = query.value(rec.indexOf("num_of_satellites")).toInt();

    strF = "UPDATE planets "
           "SET num_of_satellites = '" + QString::number(++num_of_satellites) + "' "
           "WHERE name = '" + ui->planet->text() + "';";

    if (!query.exec(strF))  {
        qDebug() << "Unable to update data in planets, on_add_clicked() method of Add_satellite class";
        QMessageBox::critical(this, "Error!", "Unable to update data in planets!");
        close();
    }

    emit add_clicked(ui->name->text(), ui->planet->text());
    close();
}
