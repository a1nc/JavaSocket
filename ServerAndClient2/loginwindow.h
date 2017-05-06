#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QDebug>
#include <QObject>
#include <iostream>
#include <string>
#include <string.h>
#include "numberdefine.h"
#include "typedefine.h"
#include "userwindow.h"

namespace Ui {
  class Loginwindow;
}

class Loginwindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit Loginwindow(QWidget *parent = 0);
  ~Loginwindow();
  void FuncGetName(void);
  void FuncGetPswd(void);
  void FuncShowUserwindow(QString);
  void FuncFormatLogin(void);
  void FuncShowError(QString);

  //
  QPushButton *ButtonLogin;
  Userwindow uw;
  //

signals:
   void SigPostNameStr(const QString &Name);
   void SigPostPswdStr(const QString &Pswd);
   void SigPostLoginStr(const QString &LoginStr);

private:
  Ui::Loginwindow *ui;
  QLineEdit *LineUserName;
  QLineEdit *LineUserPswd;
  QLabel *LabelUserName;
  QLabel *LabelUserPswd;
  QLabel *LabelLoginError;
  QString Name;
  QString Pswd;
  QString LoginStr;
  char TempStr[BUFFER_LENGTH];
};

#endif // LOGINWINDOW_H
