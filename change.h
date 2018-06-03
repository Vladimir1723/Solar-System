#ifndef CHANGE_H
#define CHANGE_H

#include <QDialog>
#include <QtWidgets>
#include <QtSql>

namespace Ui {
class Change;
}

class Change : public QDialog
{
    Q_OBJECT

public:
    explicit Change(QWidget *parent = 0);
    void init(QString age, QString star_name, QString composition);

    ~Change();

private slots:

    void on_cancel_clicked();
    void on_age_textChanged(const QString& arg);
    void on_star_name_textChanged(const QString& arg);
    void on_composition_textChanged(const QString &arg);
    void on_ok_clicked();

private:
    Ui::Change *ui;



signals:
    void ok_clicked();
};

#endif // CHANGE_H
