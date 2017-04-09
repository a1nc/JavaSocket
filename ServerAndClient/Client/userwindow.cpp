#include "userwindow.h"
#include "ui_userwindow.h"
#include "talkwindow.h"
#include <QDebug>
#include <QObject>
#include "socketclient.h"

UserWindow::UserWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserWindow)
{
    ui->setupUi(this);
    treeView = new QTreeView(this);
    treeView->setGeometry(20,50,300,600);
    model = new QStandardItemModel(3,1);
    model->setHeaderData(0,Qt::Horizontal,"Friends&Group");
    //model->setHeaderData(1,Qt::Horizontal,"Friends&Group");
    QStandardItem *title_0 = new QStandardItem("Friends");
    QStandardItem *title_1 = new QStandardItem("Groups");

    QStandardItem *f_item0 = new QStandardItem("atnc");
    QStandardItem *f_item1 = new QStandardItem("this");
    QStandardItem *f_item3 = new QStandardItem("inta");
    QStandardItem *f_item4 = new QStandardItem("lola");
    QStandardItem *f_item5 = new QStandardItem("1das1");
    QStandardItem *f_item6 = new QStandardItem("asdasf");
    QStandardItem *g_item0 = new QStandardItem("a1nc");

    title_0->appendRow(f_item0);
    title_0->appendRow(f_item1);
    title_0->appendRow(f_item3);
    title_0->appendRow(f_item4);
    title_0->appendRow(f_item5);
    title_0->appendRow(f_item6);
    title_1->appendRow(g_item0);

    model->setItem(0,0,title_0);
    model->setItem(1,0,title_1);

    treeView->setModel(model);
    treeView->setEditTriggers(0);
    treeView->expandAll();

    QObject::connect(treeView,&QTreeView::doubleClicked,this,&UserWindow::TalkWindowShow);


}

UserWindow::~UserWindow()
{
    delete ui;
}


void UserWindow::TalkWindowShow(const QModelIndex &index)
{
    QString A = index.data().toString();
    talkwindow *w = new talkwindow(this);
    w->NameStr = A ;//+ "@";
    qDebug()<<w->NameStr;
    QObject::connect(w->Sbutton,&QPushButton::clicked,w,&talkwindow::TextSend);
    QObject::connect(w,&talkwindow::MsgGet,this,&UserWindow::TalkWindowMsg);
    QObject::connect(this,&UserWindow::TWShowMsg,w,&talkwindow::TextShow);
    w->show();
}

void UserWindow::TalkWindowMsg(QString A)
{
  qDebug()<<A;
  //QString B= talkwindow::NameStr + A;
  UserWindow::TWSendMsg(A);
}

void UserWindow::TWGetMsg(QString A)
{
  qDebug()<<A;
  UserWindow::TWShowMsg(A);
}
