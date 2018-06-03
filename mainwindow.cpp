#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   
    if (!this->createConnection())
    {
        QMessageBox::critical(this, "Error!", "The database was not opened! We are sorry, good luck...");
        qDebug() << "The database was not opened! (constructor of MainWindow)";
    }

    ui->setupUi(this);
    ui->main_label->setText("<h1><b><i> <font color = white>The Solar System</font><i></b></h1>");
    ui->text->setText(_bigText);

    _pix.load(":/1024px-Planets2013.svg.png");
    _pix = _pix.scaled(400, 400, Qt::KeepAspectRatio);

    ui->picture->resize(_pix.size());
    ui->picture->setPixmap(_pix);

    QSqlQuery query_system;

    if (!query_system.exec("SELECT name, age, num_of_planet, composition "
                    "FROM star"))   {
        QMessageBox::critical(this, "Error!", "Unable to execute query!");
        qDebug() << "Unable to execute query - exiting (constructor of MainWindow)";
    }

    QSqlRecord rec = query_system.record();

    query_system.first();
    _attr.star_name = query_system.value(rec.indexOf("name")).toString();
    _attr.age = query_system.value(rec.indexOf("age")).toString();
    _attr.num_of_planet = query_system.value(rec.indexOf("num_of_planet")).toString();
    _attr.star_composition = query_system.value(rec.indexOf("composition")).toString();

    ui->attributes->setText("<h3><b><i><font color = white>Age: " + _attr.age + " billions years</font></i></b></h3>"
                            "<h3><b><i><font color = white>Num of planets: " + _attr.num_of_planet + "</font></i></b></h3>"
                            "<h3><b><i><font color = white>Star: " + _attr.star_name + "</font></i></b></h3>"
                            "<h3><b><i><font color = white>Composition: " + _attr.star_composition + "</font></i></b></h3>"
                            );

    ui->treeWidget->setHeaderLabel("System");
    ui->treeWidget->setSortingEnabled(false);
    ptwgItem = new QTreeWidgetItem(ui->treeWidget);
    ptwgItem->setText(0, _attr.star_name);

    ptwgItemPlanet = nullptr;
    QSqlQuery planet_query;
    if (!planet_query.exec("SELECT name, num_of_satellites "
                     "FROM planets "))  {
        qDebug() << "Unable to select data from planet table (constructor of MainWindow)";
        QMessageBox::critical(this, "Error", "Unable to select data from planet table!");
    }
    QSqlRecord planet_rec = planet_query.record();
    while (planet_query.next())    {
        ptwgItemPlanet = new QTreeWidgetItem(ptwgItem);
        ptwgItemPlanet->setText(0, planet_query.value(planet_rec.indexOf("name")).toString());
        if (planet_query.value(planet_rec.indexOf("num_of_satellites")).toInt()) {
            QSqlQuery satellite_query;
            if (!satellite_query.exec("SELECT name "
                                      "FROM satellites "
                                      "WHERE planet = '" + planet_query.value(planet_rec.indexOf("name")).toString() + "';"))    {
                qDebug() << "All bad...and sad..."
                            "We cannot read fucking data from fucking base (satellites), (constructor of ManWindow)";
                QMessageBox::critical(this, "Error!", "Unable to reading data from satellite table!");
            }
            QSqlRecord satellite_rec = satellite_query.record();
            ptwgItemSatellite = nullptr;
            while (satellite_query.next())  {
                ptwgItemSatellite = new QTreeWidgetItem(ptwgItemPlanet);
                ptwgItemSatellite->setText(0, satellite_query.value(satellite_rec.indexOf("name")).toString());
            }
        }
    }

    ui->treeWidget->setItemExpanded(ptwgItem, true);

    setWindowTitle("Solar system");

    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(item_clicked(QTreeWidgetItem*,int)));
    connect(ui->treeWidget, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(item_clicked(QTreeWidgetItem*,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_close_clicked()
{
    close();
}

void MainWindow::on_actionInfo_triggered()
{
    QMessageBox::information(this, "Information", "To work with this program, turn on your computer and run this program");
}

void MainWindow::on_actionAuthor_triggered()
{
    QMessageBox::information(this, "About authors",
                             "<b><i><h3>The program was created by a student of the MM-15-b group:</h3></i></b>"
                             "<b><i><h3>                                             Utochkin V.Y.</h3></i></b>"
                             "<b><i><h3>                                                  Teacher:</h3></i></b>"
                             "<b><i><h3>               Associate Professor of Department of MMSP, </h3></i></b>"
                             "<b><i><h3>                                      c.ph.-m.s. Yanz A.Y.</h3></i></b>");
}

void MainWindow::item_clicked(QTreeWidgetItem* item, int column)
{
    if (item->parent() == nullptr)
    {
        Star *star = new Star(this);
        star->show();
        connect(star, SIGNAL(ok_clicked()), this, SLOT(update_star()));
        connect(star, SIGNAL(add_clicked(QString)), this, SLOT(add_planet(QString)));
    }
    else if (item->childCount() == 0 && item->parent()->parent() != nullptr)
    {
        Satellite *satellite = new Satellite(this, item->text(0));
        satellite->show();
        connect(satellite, SIGNAL(ok_clicked(QString,QString)), this, SLOT(update_satellite(QString,QString)));
        connect(satellite, SIGNAL(delete_clicked(QString)), this, SLOT(delete_satellite(QString)));
    }
    else
    {
        Planet* planet = new Planet(this, item->text(0));
        planet->show();
        connect(planet, SIGNAL(ok_clicked(QString,QString)), this, SLOT(update_planet(QString,QString)));
        connect(planet, SIGNAL(add_clicked(QString, QString)), this, SLOT(add_satellite(QString, QString)));
        connect(planet, SIGNAL(delete_cliked(QString)), this, SLOT(delete_planet(QString)));
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_change_clicked()
{
    Change *change = new Change(this);
    change->init(_attr.age, _attr.star_name, _attr.star_composition);
    change->show();

    connect(change, SIGNAL(ok_clicked()), this, SLOT(update_system()));
}

void MainWindow::update_system ()
{
    QSqlQuery query;

    if (!query.exec("SELECT name, age, num_of_planet, composition "
                    "FROM star"))   {
        QMessageBox::critical(this, "Error!", "Error in reading data from the base!");
        qDebug() << "Error in reading data from the base (in method update_data(), class MainWindow)";
    }

    QSqlRecord rec = query.record();
    query.first();
    _attr.star_name = query.value(rec.indexOf("name")).toString();
    _attr.age = query.value(rec.indexOf("age")).toString();
    _attr.num_of_planet = query.value(rec.indexOf("num_of_planet")).toString();
    _attr.star_composition = query.value(rec.indexOf("composition")).toString();

    ptwgItem->setText(0, _attr.star_name);

    ui->attributes->setText("<h3><b><i><font color = white>Age: " + _attr.age + " billions years</font></i></b></h3>"
                            "<h3><b><i><font color = white>Num of planets: " + _attr.num_of_planet + "</font></i></b></h3>"
                            "<h3><b><i><font color = white>Star: " + _attr.star_name + "</font></i></b></h3>"
                            "<h3><b><i><font color = white>Composition: " + _attr.star_composition + "</font></i></b></h3>"
                            );
}

void MainWindow::update_star()
{
    QSqlQuery query;

    if (!query.exec("SELECT age, composition "
                    "FROM sun"))   {
        QMessageBox::critical(this, "Error!", "Error in reading data from the base!");
        qDebug() << "Error in reading data from the base (in method update_star(), class MainWindow)";
    }

    QSqlRecord rec = query.record();
    query.first();

    _attr.age = query.value(rec.indexOf("age")).toString();
    _attr.star_composition = query.value(rec.indexOf("composition")).toString();

    if (!query.exec("SELECT num_of_planet "
                    "FROM star"))   {
        QMessageBox::critical(this, "Error!", "Error in reading data from the base!");
        qDebug() << "Error in reading data from the base (in method update_star(), class MainWindow)";
    }
    rec = query.record();
    query.first();
    _attr.num_of_planet = query.value(rec.indexOf("num_of_planet")).toString();

    ui->attributes->setText("<h3><b><i><font color = white>Age: " + _attr.age + " billions years</font></i></b></h3>"
                            "<h3><b><i><font color = white>Num of planets: " + _attr.num_of_planet + "</font></i></b></h3>"
                            "<h3><b><i><font color = white>Star: " + _attr.star_name + "</font></i></b></h3>"
                            "<h3><b><i><font color = white>Composition: " + _attr.star_composition + "</font></i></b></h3>"
                            );

    QString strF = "UPDATE star "
                   "SET age = '%1', "
                   "composition = '%2'",
            str = strF.arg(_attr.age)
                      .arg(_attr.star_composition);

    if (!query.exec(str))   {
        QMessageBox::critical(this, "Error!", "Error in updating data in the base!");
        qDebug() << "Error in updating data from the base (star) (in method update_star(), class MainWindow)";
    }
}

void MainWindow::update_planet(const QString& old_name, const QString& new_name)
{
    for (int i = 0; i < ptwgItem->childCount(); i++)    {
        if (ptwgItem->child(i)->text(0) == old_name)    {
            ptwgItem->child(i)->setText(0, new_name);
            break;
        }
    }
    QSqlQuery query;
    if (!query.exec("UPDATE satellites "
                    "SET planet ='" + new_name + "' "
                    "WHERE planet = '" + old_name + "';"))  {
        qDebug() << "Unable to update data in satellites - update_planet(), MainWindow";
        QMessageBox::critical(this, "Error!", "Unable to updating data in satellite table!");
    }
}

void MainWindow::update_satellite(const QString &old_name, const QString &new_name)
{
    for (int i = 0; i < ptwgItem->childCount(); i++)
    for (int j = 0; j < ptwgItem->child(i)->childCount(); j++)
        if (ptwgItem->child(i)->child(j)->text(0) == old_name)  {
            ptwgItem->child(i)->child(j)->setText(0, new_name);
        }
}

void MainWindow::delete_satellite(const QString &name)
{
    for (int i = 0; i < ptwgItem->childCount(); i++)
    for (int j = 0; j < ptwgItem->child(i)->childCount(); j++)
        if (ptwgItem->child(i)->child(j)->text(0) == name)  {
            ptwgItem->child(i)->removeChild(ptwgItem->child(i)->child(j));
        }
}

void MainWindow::add_satellite(const QString &name, const QString& planet)
{
    QTreeWidgetItem *item = nullptr;
    for (int i = 0; i < ptwgItem->childCount(); i++)    {
        if (ptwgItem->child(i)->text(0) == planet)  {
            item = new QTreeWidgetItem(ptwgItem->child(i));
            item->setText(0, name);
            ptwgItem->child(i)->addChild(item);
            break;
        }
    }
}

void MainWindow::delete_planet(const QString &name)
{
    for (int i = 0; i < ptwgItem->childCount(); i++)
        if (ptwgItem->child(i)->text(0) == name)    {
            for (int j = 0; j < ptwgItem->child(i)->childCount(); j++)
                ptwgItem->child(i)->removeChild(ptwgItem->child(i)->child(j));
            ptwgItem->removeChild(ptwgItem->child(i));
            break;
        }
    QSqlQuery query;
    if (!query.exec("SELECT num_of_planet "
                    "FROM star "));
    query.first();
    QSqlRecord rec = query.record();
    int num = query.value(rec.indexOf("num_of_planet")).toInt();
    ui->attributes->setText("<h3><b><i><font color = white>Age: " + _attr.age + " billions years</font></i></b></h3>"
                            "<h3><b><i><font color = white>Num of planets: " + QString::number(num) + "</font></i></b></h3>"
                            "<h3><b><i><font color = white>Star: " + _attr.star_name + "</font></i></b></h3>"
                            "<h3><b><i><font color = white>Composition: " + _attr.star_composition + "</font></i></b></h3>"
                            );
}

void MainWindow::add_planet(const QString &name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ptwgItem);
    item->setText(0, name);

    QSqlQuery query;
    if (!query.exec("SELECT num_of_planet "
                    "FROM star "));
    query.first();
    QSqlRecord rec = query.record();
    int num = query.value(rec.indexOf("num_of_planet")).toInt();
    ui->attributes->setText("<h3><b><i><font color = white>Age: " + _attr.age + " billions years</font></i></b></h3>"
                            "<h3><b><i><font color = white>Num of planets: " + QString::number(num) + "</font></i></b></h3>"
                            "<h3><b><i><font color = white>Star: " + _attr.star_name + "</font></i></b></h3>"
                            "<h3><b><i><font color = white>Composition: " + _attr.star_composition + "</font></i></b></h3>"
                            );
}

bool MainWindow::createConnection()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("Solar System");
    database.setHostName("Ksenia");
    database.setUserName("Vlad");
    database.setPassword("1723");
    if (!database.open())
    {
        qDebug() << "Cannot open database!" << database.lastError();
        return false;
    }
    qDebug() << "The database was opened!!! Yahooo!!!";
    return true;
}

void MainWindow::on_actionEdit_triggered()
{
    this->on_change_clicked();
}
