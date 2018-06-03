#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtSql>
#include <star.h>
#include <planet.h>
#include <change.h>
#include <satellite.h>

namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{

    Q_OBJECT

private:

    struct _Attributes
    {
        QString age;
        QString num_of_planet;
        QString star_name, star_composition;
    };

    _Attributes _attr;

    const QString _bigText =
            "<h3><b><i><font color = white>The Solar System is the </font></i></b></h3>"
            "<h3><b><i><font color = white>gravitationally </font></i></b></h3>"
            "<h3><b><i><font color = white>bound system comprising </font></i></b></h3>"
            "<h3><b><i><font color = white>the Sun and the </font></i></b></h3>"
            "<h3><b><i><font color = white>objects that orbit </font></i></b></h3>"
            "<h3><b><i><font color = white>it, either directly </font></i></b></h3>"
            "<h3><b><i><font color = white>or indirectly. Of those </font></i></b></h3>"
            "<h3><b><i><font color = white>objects that orbit</font></i></b></h3>"
            "<h3><b><i><font color = white> the Sun directly, </font></i></b></h3>"
            "<h3><b><i><font color = white>the largest eight are </font></i></b></h3>"
            "<h3><b><i><font color = white>the planets, with </font></i></b></h3>"
            "<h3><b><i><font color = white>the remainder being</font></i></b></h3>"
            "<h3><b><i><font color = white> smaller objects, </font></i></b></h3>"
            "<h3><b><i><font color = white>such as dwarf planets</font></i></b></h3>"
            "<h3><b><i><font color = white> and small Solar</font></i></b></h3>"
            "<h3><b><i><font color = white> System bodies. Of </font></i></b></h3>"
            "<h3><b><i><font color = white>the objects that orbit </font></i></b></h3>"
            "<h3><b><i><font color = white>the Sun indirectly, </font></i></b></h3>"
            "<h3><b><i><font color = white>the moons, two are</font></i></b></h3>"
            "<h3><b><i><font color = white> larger than the smallest </font></i></b></h3>"
            "<h3><b><i><font color = white>planet, Mercury.</font></i></b></h3>";

    QPixmap _pix;

    static bool createConnection();

     QTreeWidgetItem* ptwgItem;
     QTreeWidgetItem *ptwgItemPlanet;
     QTreeWidgetItem *ptwgItemSatellite;

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_close_clicked();

    void on_actionInfo_triggered();

    void on_actionAuthor_triggered();

    void item_clicked(QTreeWidgetItem*, int);
    void on_actionExit_triggered();

    void on_change_clicked();

    void update_system();

    void update_star();

    void update_planet(const QString& old_name, const QString& new_name);

    void update_satellite (const QString& old_name, const QString& new_name);

    void delete_satellite(const QString& name);

    void add_satellite(const QString& name, const QString& name_of_planet);

    void delete_planet(const QString& name);

    void add_planet(const QString& name);

    void on_actionEdit_triggered();

private:

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
