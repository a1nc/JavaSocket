#include "talkwindow.h"
#include "ui_talkwindow.h"

talkwindow::talkwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::talkwindow)
{
    ui->setupUi(this);
}

talkwindow::~talkwindow()
{
    delete ui;
}
