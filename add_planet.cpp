#include "add_planet.h"
#include "ui_add_planet.h"

Add_planet::Add_planet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_planet)
{
    ui->setupUi(this);
    ui->add->setEnabled(false);

    setWindowTitle("Add planet");
}

Add_planet::~Add_planet()
{
    delete ui;
}

void Add_planet::on_cancel_clicked()
{
    close();
}

void Add_planet::on_name_textChanged(const QString &arg1)
{
    ui->add->setEnabled( !arg1.isEmpty() && !ui->length_of_equator->text().isEmpty() &&
                         !ui->composition->text().isEmpty() );
}

void Add_planet::on_length_of_equator_textChanged(const QString &arg1)
{
    ui->add->setEnabled( !arg1.isEmpty() && !ui->name->text().isEmpty()
                         && !ui->composition->text().isEmpty() );
}

void Add_planet::on_composition_textChanged(const QString &arg1)
{
    ui->add->setEnabled( !arg1.isEmpty() && !ui->name->text().isEmpty()
                         && !ui->length_of_equator->text().isEmpty() );
}

void Add_planet::on_add_clicked()
{
    QSqlQuery query;
    QString str = "INSERT INTO planets "
                  "VALUES ('%1', '%2', '%3', '%4', '%5');",
            strF = str.arg(ui->name->text(), "Sun", "0", ui->length_of_equator->text(), ui->composition->text());
   if (!query.exec(strF))  {
        qDebug() << "Unable to insert data into planets, on_add_clicked() slot, Add_planet class";
        QMessageBox::critical(this, "Error!", "Unable to insert data into planets!");
        close();
   }

   str = "SELECT num_of_planet "
         "FROM star ";

   if (!query.exec(str))    {
       qDebug() << "Unable to read data from star, on_add_clicked() slot, Add_planet class";
       QMessageBox::critical(this, "Error!", "Unable to read data from star!");
   }

   QSqlRecord rec = query.record();
   query.first();

   int num_of_planet = query.value(rec.indexOf("num_of_planet")).toInt();

   str = "UPDATE star "
         "SET num_of_planet = '" + QString::number(++num_of_planet) + "';";

   if (!query.exec(str))    {
       qDebug() << "Unable to update data from star, on_add_clicked() slot, Add_planet class";
       QMessageBox::critical(this, "Error!", "Unable to update data from star");
   }

   emit add_clicked(ui->name->text());
   close();
}


