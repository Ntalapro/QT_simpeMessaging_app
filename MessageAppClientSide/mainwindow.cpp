#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connectButton, &QPushButton::clicked,
        this, &MainWindow::connectButton_clicked);

    connect(ui->sendButton, &QPushButton::clicked,
        this, &MainWindow::sendButton_clicked);

     connectedToHost = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::connectButton_clicked()
{
    if(!connectedToHost){ // if disconnected create a new socket and connect to host
        socket = new QTcpSocket();

        connect(socket,&QTcpSocket::connected,this,&MainWindow::socketConnected);
        connect(socket,&QTcpSocket::disconnected,this,&MainWindow::socketDisconnected);
        connect(socket,&QTcpSocket::readyRead,this,&MainWindow::socketReadyRead);

        socket->connectToHost("127.0.0.1",8001);

    }
    else{ // when connected disconnect and show message in color orange
        QString name = ui->nameInput->text();
        socket->write("<font color=' Orange'>" + name.toUtf8() + " has left the chat room.</font>");

        socket->disconnectFromHost();
    }
}

void MainWindow:: socketConnected(){

    qDebug() << "Connected to server";

    printMessage("<font color=' Blue'> Connected to sever. </font> "); //print on the widget as well

    QString name = ui->nameInput->text();
    socket->write("<font color=' Purple'>" + name.toUtf8() + " has joined the chat room.</font>");

    ui->connectButton->setText("Disconnect");
    connectedToHost = true;

}
void MainWindow:: socketDisconnected(){

    qDebug() <<"Disconnected from server.";

    printMessage("<font color=' Red'> Disconnected from sever. </font> "); //print on the widget as well

    ui->connectButton->setText("Connect");
    connectedToHost = false;

}

void MainWindow::socketReadyRead(){
    ui->chatDisplay->append(socket->readAll());
}
void MainWindow:: printMessage(QString message){
    ui->chatDisplay->append(message);
}

void MainWindow::sendButton_clicked(){
    QString name = ui->nameInput->text(); //client name
    QString message = ui->messageInput->text();//client message to server
    QString data = message;//so we can use readAll()

    socket->write("<font color=' Green'>" + name.toUtf8() + "</font>: " + data.toUtf8());

    ui->messageInput->clear();//clear message input

}


















