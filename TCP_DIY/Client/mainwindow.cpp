#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myclient.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_clicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::on_pushButton_3_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_4_clicked);

    //全局线程
    connect(&m_heart,&QTimer::timeout, this, &MainWindow::timeout);
    m_heart.setInterval(100);
    m_thread = new _Thread(this);
    m_heart.start();
}

MainWindow::~MainWindow()
{
    m_thread->wait();
    delete ui;
}


void MainWindow::on_pushButton_clicked() //connect
{
    std::string ip = "127.0.0.1";
    int port = Port;
    m_thread->connnectsocket(ip, port);

    if(m_thread->isRunning())
    {
        return;
    }
    m_thread->start();
}

void MainWindow::on_pushButton_3_clicked() //close
{
    m_thread->closesocket();
}

std::string MainWindow::tra()
{
    people::People person;
    QString qstrhead = ui->Head_->toPlainText();
    std::string strhead = qstrhead.toStdString();
    person.set_head( strhead );

    QString qstrname = ui->Name_->toPlainText();
    std::string strname = qstrname.toStdString();
    person.set_name( strname );

    QString qstrid =  ui->ID_->toPlainText();
    std::string strid = qstrid.toStdString();
    person.set_id( strid );

    QString qstremail = ui->Email_->toPlainText();
    std::string stremail = qstremail.toStdString();
    person.set_email ( stremail );

    std::string message;
    person.SerializeToString(&message);
    return message;
}

void MainWindow::on_pushButton_2_clicked() //send
//insert
{
    std::string strsend = tra();
    QString reQString = m_thread->send( strsend );
    ui->textEdit_2->setPlainText(reQString);

}

void MainWindow::on_pushButton_4_clicked() //receive
{
    QString reQString = m_thread->receive();
    re_QString = reQString;
     ui->textEdit_2->setPlainText(re_QString);
//    display();
}



void MainWindow::display()
{
    if(re_QString.isEmpty())
    {
        std::cout << "client: rev error" << std::endl;
        return ;
    }
    // receive server inform
    ui->textEdit_2->setPlainText(re_QString);

    //display:name id email
    std::string str1 = re_QString.toStdString();
    people::People p;
    p.ParseFromString(str1);

    //display
    ui->Name_->setPlainText( QString::fromStdString( p.name() ) );
    ui->ID_->setPlainText( QString::fromStdString( p.id() ) );
    ui->Email_->setPlainText( QString::fromStdString( p.email() ) );
}


void MainWindow::timeout()
{
    static int s_heartcnt = 0;
    ++s_heartcnt;
    if(s_heartcnt >100)
    {
        s_heartcnt = 0;
    }
}
