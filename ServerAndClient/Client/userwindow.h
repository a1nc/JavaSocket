#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QStandardItemModel>
#include "talkwindow.h"
#include "socketclient.h"

namespace Ui {
class UserWindow;
}

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = 0);
    ~UserWindow();
    MySocket userMS;
    QTreeView *treeView;
    void TalkWindowShow(const QModelIndex &index);
    void TalkWindowMsg(QString);
    void TWGetMsg(QString);

signals:
    void TWSendMsg(const QString &SendMsg);
    void TWShowMsg(const QString &ShowMsg);

private:
    Ui::UserWindow *ui;
    talkwindow tw;
    QStandardItemModel *model;
    QString SendMsg;
    QString ShowMsg;
};
#endif // USERWINDOW_H
