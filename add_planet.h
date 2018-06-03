#ifndef ADD_PLANET_H
#define ADD_PLANET_H

#include <QDialog>
#include <QtSql>
#include <QtWidgets>

namespace Ui {
class Add_planet;
}

class Add_planet : public QDialog
{
    Q_OBJECT

public:
    explicit Add_planet(QWidget *parent = 0);
    ~Add_planet();

private slots:
    void on_cancel_clicked();

    void on_name_textChanged(const QString &arg1);

    void on_length_of_equator_textChanged(const QString &arg1);

    void on_composition_textChanged(const QString &arg1);

    void on_add_clicked();

signals:
    void add_clicked(const QString& name);

private:
    Ui::Add_planet *ui;
};

#endif // ADD_PLANET_H
