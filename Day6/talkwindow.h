#ifndef TALKWINDOW_H
#define TALKWINDOW_H

#include <QMainWindow>

namespace Ui {
class talkwindow;
}

class talkwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit talkwindow(QWidget *parent = 0);
    ~talkwindow();

private:
    Ui::talkwindow *ui;
};

#endif // TALKWINDOW_H
