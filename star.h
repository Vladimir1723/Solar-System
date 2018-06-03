#ifndef STAR_H
#define STAR_H

#include <QDialog>
#include <QtSql>
#include <QtWidgets>
#include <add_planet.h>

namespace Ui {
class Star;
}

class Star : public QDialog
{
    Q_OBJECT

public:

    explicit Star(QWidget *parent = 0);
    ~Star();

private slots:
    void on_close_clicked();

    void on_ok_clicked();

    void on_age_value_textChanged(const QString &arg1);

    void on_composition_value_textChanged(const QString &arg1);

    void on_temperature_value_textChanged(const QString &arg1);

    void on_add_planet_clicked();
signals:
    void add_clicked(const QString& name);
private:

    Ui::Star *ui;

    struct _Attributes
    {
        QString age, name,
        temperature, composition;
    };

    _Attributes _attr;
    QPixmap _pix;

signals:
    void ok_clicked();
};

#endif // STAR_H
