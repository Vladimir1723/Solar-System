#ifndef ADD_SATELLITE_H
#define ADD_SATELLITE_H

#include <QDialog>
#include <QtSql>
#include <QtWidgets>

namespace Ui {
class Add_satellite;
}

class Add_satellite : public QDialog
{
    Q_OBJECT

public:
    explicit Add_satellite(QWidget *parent, const QString& name_of_planet);
    ~Add_satellite();

private slots:
    void on_cancel_clicked();

    void on_name_textChanged(const QString &arg1);

    void on_length_of_equator_textChanged(const QString &arg1);

    void on_add_clicked();

signals:
    void add_clicked(const QString& name_of_satellite, const QString& planet_name);

private:
    Ui::Add_satellite *ui;
};

#endif // ADD_SATELLITE_H
