#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QStandardItemModel>
#include "talkwindow.h"

namespace Ui {
class UserWindow;
}

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = 0);
    ~UserWindow();
    QTreeView *treeView;
    void TalkWindowShow(void);

private:
    Ui::UserWindow *ui;
    talkwindow tw;
    QStandardItemModel *model;
};
#endif // USERWINDOW_H
