#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include "myclient.h"
#include "_qthread.h"
#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

public:
    void timeout();
    void display();
    std::string tra();
private:
    Ui::MainWindow *ui;
    QString re_QString;
    _Thread* m_thread;
    QTimer m_heart;
};

#endif // MAINWINDOW_H
