#ifndef PLANET_H
#define PLANET_H

#include <QDialog>
#include <QtSql>
#include <QtWidgets>
#include <add_satellite.h>

namespace Ui {
class Planet;
}

class Planet : public QDialog
{
    Q_OBJECT

public:
    explicit Planet(QWidget *parent, const QString& name);
    ~Planet();

private slots:
    void on_cancel_clicked();

    void on_length_of_equator_textChanged(const QString &arg1);

    void on_composition_textChanged(const QString &arg1);

    void on_ok_clicked();

    void on_name_textChanged(const QString &arg1);

    void on_add_satellite_clicked();
    void on_add_clicked(const QString& name, const QString& planet);
    void on_Delete_clicked();

signals:
    void ok_clicked(const QString& old_name, const QString& new_name);
    void add_clicked(const QString& name_of_satellite, const QString& name_of_planet);
    void delete_cliked(const QString& name);
private:
    Ui::Planet *ui;

    struct _Attributes  {
        QString name, length_of_equator,
        composition, num_of_satellites;
    };

    _Attributes _attr;
    QPixmap _pix;
};

#endif // PLANET_H
