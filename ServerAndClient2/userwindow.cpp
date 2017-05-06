#include "userwindow.h"
#include "ui_userwindow.h"
#include "numberdefine.h"
#include "typedefine.h"

Userwindow::Userwindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Userwindow)
{
  ui->setupUi(this);
  this->setFixedSize(340,700);
  char PureUserName[NAME_LENGTH+2];
  memset(PureUserName,0,NAME_LENGTH+2);
  int Count = 0;
  for(int i=0;i<UserName.length();i++)
    {
      if(UserName[i] == '+')
      {
         break;
      }
      Count++;
    }
  strncpy(PureUserName,UserName.toLatin1().data(),Count);
  std::cout<<UserName.toLatin1().data()<<"   AAAA:"<<PureUserName<<std::endl;
  this->setWindowTitle(QString::fromStdString(PureUserName));
  TreeFriendList = new QTreeView(this);
  TreeFriendList->setGeometry(20,50,300,600);

  Model = new QStandardItemModel(3,1);
  Model->setHeaderData(0,Qt::Horizontal,"Friend List");

  Title = new QStandardItem("Friend");
  Model->setItem(0,0,Title);

  TreeFriendList->setModel(Model);
  TreeFriendList->setEditTriggers(0);
  TreeFriendList->expandAll();
  QObject::connect(TreeFriendList,&QTreeView::doubleClicked,this,&Userwindow::FuncTalkWindowShow);
}

Userwindow::~Userwindow()
{
  delete ui;
}

void Userwindow::FuncUserListInit(QString InputString)
{
    char Name[NAME_LENGTH+2];
    char UserList[BUFFER_LENGTH];
    memset(UserList,0,BUFFER_LENGTH);
    strcpy(UserList,InputString.toLatin1().data());
    int Length = InputString.length()-8;
    int Count = Length/20;
    for(int i=0;i<Count;i++)
      {
          memset(Name,0,NAME_LENGTH+2);
          strncpy(Name,UserList+i*20+8,20);
          int j = 0;
          for(j=0;j<20;j++)
            {
              if(Name[j]=='+')
                {
                  break;
                }
            }
          char TempName[NAME_LENGTH+2];
          memset(TempName,0,NAME_LENGTH+2);
          strncpy(TempName,Name,j);
          FriendList[i] = new QStandardItem(QString::fromStdString(TempName));
          Title->appendRow(FriendList[i]);
      }
    for(int i=0;i<BUFFER_LENGTH;i++)
      {
        memset(UserMsgRecord[i],0,BUFFER_LENGTH+50);
      }
}

void Userwindow::FuncTalkWindowShow(const QModelIndex &index)
{
  QString SendTo = index.data().toString();
  std::cout<<"Test"<<std::endl;
  Talkwindow *w = new Talkwindow(this);
  w->FuncSetName(SendTo);
  w->setWindowTitle(SendTo);
  w->FuncSetUserName(UserName);
  QObject::connect(w->ButtonSend,&QPushButton::clicked,w,&Talkwindow::FuncSendText);
  QObject::connect(w,&Talkwindow::SigSendText,this,&Userwindow::FuncTalkwindowSendMsg);
  QObject::connect(this,&Userwindow::SigReceiveMsgToPost,w,&Talkwindow::FuncShowToScrren);
  Userwindow::FuncOpenTalkWindow();
  w->show();
}
