#include "../inc/notka.h"
#include "ui_notka.h"


Notka::Notka(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::Notka)
{
        ui->setupUi(this);
}

Notka::~Notka()
{
        delete ui;
}
