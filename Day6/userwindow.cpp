#include "userwindow.h"
#include "ui_userwindow.h"
#include "talkwindow.h"
#include <QDebug>
#include <QObject>

void Test(void);

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

    QStandardItem *f_item0 = new QStandardItem("Nico");
    QStandardItem *f_item1 = new QStandardItem("Jack");
    QStandardItem *f_item3 = new QStandardItem("Nico");
    QStandardItem *f_item4= new QStandardItem("Jack");
    QStandardItem *f_item5 = new QStandardItem("Nico");
    QStandardItem *f_item6 = new QStandardItem("Jack");
    QStandardItem *g_item0 = new QStandardItem("NUAA");

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
   // QObject::connect(treeView,&QTreeView::doubleClicked,&Test);
}

UserWindow::~UserWindow()
{
    delete ui;
}

void Test(void)
{
    qDebug()<<"TEST";
}

void UserWindow::TalkWindowShow(void)
{
    talkwindow *t1 = new talkwindow(this);
    t1->show();
}
