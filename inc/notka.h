#ifndef NOTKA_H
#define NOTKA_H

#include <QMainWindow>


namespace Ui {
class Notka;
}

class Notka : public QMainWindow
{
        Q_OBJECT

public:
        explicit Notka(QWidget *parent = 0);
        ~Notka();

private:
        Ui::Notka *ui;
};

#endif // NOTKA_H
