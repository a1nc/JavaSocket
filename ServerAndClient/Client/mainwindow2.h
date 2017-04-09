#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QMainWindow>
#include <QPushButton>
#include "mainwindow2.h"
#include <QComboBox>
#include <QListView>
#include <QStringListModel>
#include <QTreeView>
#include <QStandardItemModel>

namespace Ui {
class MainWindow2;
}

class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow2(QWidget *parent = 0);
    ~MainWindow2();

private:
    Ui::MainWindow2 *ui;
    QPushButton *button;
    QComboBox *comboBox;
    QListView *listView;
    QStringListModel *model;
    QTreeView *treeView;
    QStandardItemModel *tmodel;
};

#endif // MAINWINDOW2_H
