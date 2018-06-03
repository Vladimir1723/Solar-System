#ifndef SATELLITE_H
#define SATELLITE_H

#include <QDialog>
#include <QtWidgets>
#include <QtSql>

namespace Ui {
class Satellite;
}

class Satellite : public QDialog
{
    Q_OBJECT

public:
    explicit Satellite(QWidget *parent, const QString& name);
    ~Satellite();

private slots:
    void on_cancel_clicked();

    void on_name_textChanged(const QString &arg1);

    void on_length_of_equator_textChanged(const QString &arg1);

    void on_ok_clicked();

    void on_Delete_clicked();

signals:
    void ok_clicked(const QString& old_name, const QString& new_name);
    void delete_clicked(const QString& name);

private:
    Ui::Satellite *ui;

    struct _Attributes  {
        QString name, length_of_equator,
        planet;
    };

    _Attributes _attr;
};

#endif // SATELLITE_H
