// myrobot.cpp

#include "myrobot.h"

MyRobot::MyRobot(QObject *parent) : QObject(parent) {
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = 0x0; //vitesse gauche 2 et 3 (240 maxi)
    DataToSend[3] = 0x0;
    DataToSend[4] = 0x0; //vitesse droite 4 et 5 (240 maxi)
    DataToSend[5] = 0x0;
    DataToSend[6] = 0x0;
    /* 8 bits decomposés comme suit
     * 8(128) :
     * 7
     * 6
     * 5
     * 4 Vitesse PID 50ms (0)
     * 3 Pas utilisé
     * 2    Pas utilisé
     * 1    Pas utilisé
     *
     * */
    DataToSend[7] = 0x0; //Pas utilisé pour le TCP
    DataToSend[8] = 0xFE;
    DataReceived.resize(21);
    TimerEnvoi = new QTimer();
    // setup signal and slot
    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot())); //Send data to wifibot timer
}


void MyRobot::doConnect() {
    socket = new QTcpSocket(this); // socket creation
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    qDebug() << "connecting..."; // this is not blocking call
    //socket->connectToHost("LOCALHOST", 15020);
    socket->connectToHost("10.188.194.135", 15020); // connection to wifibot   192.168.1.106
    // we need to wait...
    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
        return;
    }
    TimerEnvoi->start(75);

}

void MyRobot::disConnect() {
    TimerEnvoi->stop();
    socket->close();
    this->disconnected();
}

void MyRobot::connected() {
    qDebug() << "connected..."; // Hey server, tell me about you.
}

void MyRobot::disconnected() {
    qDebug() << "disconnected...";
}

void MyRobot::bytesWritten(qint64 bytes) {
    qDebug() << bytes << " bytes written...";
    //qDebug() << DataToSend[0] << DataToSend[1] << DataToSend[2];
    this->readyRead();
}

void MyRobot::readyRead() {
    qDebug() << "reading..."; // read the data from the socket
    DataReceived = socket->readAll();
    emit updateUI(DataReceived);
    qDebug() << (int)DataReceived[0] <<(int) DataReceived[1] << (int)DataReceived[2];
}

void MyRobot::MyTimerSlot() {
    qDebug() << "Timer...";
    qDebug() << DataToSend.toHex().toUpper();

    while(Mutex.tryLock());
    socket->write(DataToSend);
    Mutex.unlock();
}



