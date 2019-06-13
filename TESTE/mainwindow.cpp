#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include "op.h"
#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);




}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushButton_clicked()
{
   QString password;
   username=ui->identifiantLineEdit->text();
   password=ui->motDePasseLineEdit->text();
   qDebug()<<username;
   if((username=="Anais"&&password=="mardama") ||(username=="Primous"&&password=="Primous") || (username=="Barthlemy"&&password=="herymann") || (username=="gihnac"&&password=="herymann") ){

        OP Dialog(this);
       Dialog.setModal(true);
       Dialog.exec();

   }else{
       QMessageBox msg;
        msg.setText("Connexion");
       msg.setText("Vous n'êtes pas autorisé à vous connecter");
       msg.exec();
   }

}
