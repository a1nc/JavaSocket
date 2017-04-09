#include "talkwindow.h"
#include "ui_talkwindow.h"
#include "socketclient.h"
#include <QPushButton>
#include <QDebug>
#include <QApplication>

talkwindow::talkwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::talkwindow)
{
    ui->setupUi(this);
    //Middle text Window
    textEdit = new QTextEdit(this);
    textEdit->setGeometry(QRect(10,20,420,230));
    //Edit Msg Window
    MsgSendEdit = new QTextEdit(this);
    MsgSendEdit->setGeometry(QRect(10,300,420,50));

    Sbutton = new QPushButton("Send Msg",this);
    Sbutton->setGeometry(QRect(330,360,100,25));
}

talkwindow::~talkwindow()
{
    delete ui;
}

void talkwindow::TextShow(QString A)
{
        qDebug()<<"    "<<talkwindow::NameStr;
        qDebug()<<"    "<<A;
        if(A.contains(talkwindow::NameStr))
          {
            textEdit->append(A);
          }
}

void talkwindow::TextShowMe(QString A)
{
  textEdit->append(A);
}

QString talkwindow::TextSend()
{
    QString SS;
    SS = MsgSendEdit->toPlainText();
    TextShowMe(SS);
    talkwindow::MsgGet(talkwindow::NameStr + "@" + SS);
    return SS;
}
