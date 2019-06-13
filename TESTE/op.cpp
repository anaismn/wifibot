#include "op.h"
#include "ui_op.h"
#include <QMessageBox>
#include <QGraphicsView>
#include <QMediaPlayer>
OP::OP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OP)
{
    ui->setupUi(this);

    /*if(username=="Anais"){
        QMediaPlayer * music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/anais.mp4"));
        music->play();
    }
    else if(username=="Primous"){
        QMediaPlayer * music = new QMediaPlayer();
        music->setMedia(QUrl("qrc:/primous.mp4"));
        music->play();

    }else if(username=="ginhac"){
    }
    else{

    }*/
    QMediaPlayer * music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/primous.mp4"));
    music->play();
    ui->pushButton->setStyleSheet("background-color:");

    //Initialisation du controle des touches
    wifibot = new bool[4];
    for(int i=0 ; i<4 ; i++)
    {
        wifibot[i]=false;
    }

    ui->batterieBar->setValue(0);
    ui->energie->display(12);
    QTimer *timer= new QTimer();
    timer->connect(timer,SIGNAL(timeout()),this,SLOT(miseAJour()));
    timer->start(2000);

    //Envoie des données sur le robot
    QTimer *timer2= new QTimer();
    timer2->setInterval(25);
    timer2->connect(timer2, SIGNAL(timeout()),this,SLOT(envoi()));
    timer2->start(25);


}

OP::~OP()
{
    delete ui;
    this->robot.disConnect();
}

void OP::on_pushButton_2_clicked()
{

    this->robot.doConnect();
    ui->pushButton_2->setStyleSheet("background-color: rgb(0, 170, 0);");
    ui->pushButton->setStyleSheet("background-color:");
    ui->led->setStyleSheet("background-color: rgb(0, 170, 0);");
    ui->led->setToolTip("SUPER!! VOUS ETES AU ROBOT");

    this->view = new QWebEngineView(ui->cameraView);
    this->view->load(QUrl("http://192.168.1.106:8080/?action=stream"));
    this->view->resize(341,311);
    this->view->show();
    QMessageBox msg;
     msg.setText("Connexion");
    msg.setText("Coonexion réussie au robot");
    msg.exec();
    this->demarrer=true;

}

void OP::on_pushButton_clicked()
{
    this->robot.disConnect();
    ui->pushButton_2->setStyleSheet("background-color:");
    ui->pushButton->setStyleSheet("background-color:rgb(255, 0, 0)");
    ui->led->setStyleSheet("background-color: rgb(255, 0, 0);");
    ui->led->setToolTip("Oups!! Deconnecté");
    QMessageBox msg;
    msg.setText("Deconnexion");
    msg.setText("Vous n'êtes plus connecté au robot..");
    msg.exec();
            this->demarrer=false;
    //timer2->stop();
}

void OP::on_avancer_clicked()
{

     unsigned char* castData=  (unsigned char*)this->robot.DataToSend.data();

    //Control de la vitesse tics max 240
   /* if(this->vitesse<240){
        this->vitesse+=1;
    }*/

    //test de l'exemple
     this->robot.DataToSend[0]=0xff;

    this->robot.DataToSend[6]=0x50;
    this->robot.DataToSend[4]=0x3c;
    this->robot.DataToSend[2]=0x3c;
     this->robot.DataToSend[3]=0x3c;
     this->robot.DataToSend[5]=0x3c;
    qDebug()<<"Datatosend[0]              :--------"<<dtobin(castData[0]);
    qDebug()<<"Datatosend[1]              :--------"<<dtobin(castData[1]);
    qDebug()<<"Datatosend[2]              :--------"<<dtobin(castData[2]);
    qDebug()<<"Datatosend[3]              :--------"<<dtobin(castData[3]);
    qDebug()<<"Datatosend[4]              :--------"<<dtobin(castData[4]);
    qDebug()<<"Datatosend[5]              :--------"<<dtobin(castData[5]);
    qDebug()<<"Datatosend[6]              :--------"<<dtobin(castData[6]);
    qDebug()<<"Datatosend[8]              :--------"<<dtobin(castData[8]);
    short crc=Crc16(castData+1,6);
    this->robot.DataToSend[7]=(char)crc;
    this->robot.DataToSend[8]=(char)(crc>>8);

     qDebug()<<"Datatosend[7]              :--------"<<(crc);
     qDebug()<<"Datatosend[8]              :--------"<<(crc>>8);

     qDebug()<<robot.DataToSend.toHex().toUpper();

}
void OP::on_reculer_clicked()
{
    //Control de la vitesse tics max 240

   /*
    *  if(this->vitesse>0){
        this->vitesse-=1;
    }
    //test de l'exemple
    this->robot.DataToSend[6]=0x50;
    this->robot.DataToSend[4]=0x78;
    this->robot.DataToSend[2]=0x78;

    */
   // this->robot.MyTimerSlot();

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  MOUVEMENT DU WIFIBOT
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void OP::Deplacement(unsigned char leftspeed, unsigned char rightspeed, int Direction)
{
   /* //TRAME A ENVOYER
    buffer->clear();
    buffer->append((char)255);//char1 is 255
    buffer->append((char)0x07);//char2 is 0x07
    buffer->append(leftspeed);//char3 & char4 are the leftspeed control
    buffer->append((char)0);//char4
    buffer->append(rightspeed);//char5 & char6 are the rightspeed control
    buffer->append((char)0);//char6
    //Nous n'utiliserons que deux bits du char7 :
        //le bit 6 (64) : lorsqu'il est à 1 les roues de gauches avancent, sinon elles reculent
        //le bit 4 (16) : lorsqu'il est à 1 les roues de droites avancent, sinon elles reculent

    //test de l'exemple*/

     this->robot.DataToSend[0]=0xff;
     this->robot.DataToSend[2]=leftspeed;
     this->robot.DataToSend[3]=leftspeed;
     this->robot.DataToSend[4]=rightspeed;
     this->robot.DataToSend[5]=rightspeed;

    if(Direction==1){//AVANCER
      //  buffer->append((char)0b01010000);//pour avancer on met 11
        this->robot.DataToSend[6]=(unsigned char)0b01010000;
    }
    else if(Direction==2){//Gauche
        //buffer->append((char)0b00010000);//pour aller à droite on ne fait avancer que les roues droites donc 01
          this->robot.DataToSend[6]=(char)0b11010000;
    }
    else if(Direction==3){//RECULER
      //  buffer->append((char)0b00000000);//pour reculer on met 00
        this->robot.DataToSend[6]=(char)0b00000000;
    }
    else if(Direction==4){//droite
       // buffer->append((char)0b01000000);//pour aller à droite on ne fait avancer que les roues gauches donc 10
         this->robot.DataToSend[6]=(char)0b01110000;
    }

    //Calcul du CRC
   // unsigned short crc = Crc16((unsigned char* )buffer->constData(), buffer->length());
   // buffer->append((char)crc);
   // buffer->append((char)(crc>>8));
      unsigned char* castData=  (unsigned char*)this->robot.DataToSend.data();
      short crc=Crc16(castData+1,6);
      this->robot.DataToSend[7]=(char)crc;
      this->robot.DataToSend[8]=(char)(crc>>8);

      qDebug()<<"Datatosend[0]              :--------"<<dtobin(castData[0]);
      qDebug()<<"Datatosend[1]              :--------"<<dtobin(castData[1]);
      qDebug()<<"Datatosend[2]              :--------"<<dtobin(castData[2]);
      qDebug()<<"Datatosend[3]              :--------"<<dtobin(castData[3]);
      qDebug()<<"Datatosend[4]              :--------"<<dtobin(castData[4]);
      qDebug()<<"Datatosend[5]              :--------"<<dtobin(castData[5]);
      qDebug()<<"Datatosend[6]              :--------"<<dtobin(castData[6]);
       qDebug()<<"Datatosend[7]              :--------"<<dtobin(castData[7]);
      qDebug()<<"Datatosend[8]              :--------"<<dtobin(castData[8]);
      qDebug()<<robot.DataToSend.toHex().toUpper()<<"\n";

}

void OP::envoi()
{
    if(wifibot[0]&&!wifibot[1]&&!wifibot[2]&&!wifibot[3]){//avant
        qDebug()<<"Avant appuyé"<<"\n";
        Deplacement(450,450,1);
    }
    else if(!wifibot[0]&&!wifibot[1]&&wifibot[2]&&!wifibot[3]){//arriere
         qDebug()<<"Arriere appuyé"<<"\n";
        Deplacement(450,450,3);
    }
    else if(!wifibot[0]&&wifibot[1]&&!wifibot[2]&&!wifibot[3]){//gauche
         qDebug()<<"Avant gauche"<<"\n";
        Deplacement(450,450,2);
    }
    else if(!wifibot[0]&&!wifibot[1]&&!wifibot[2]&&wifibot[3]){//droite
         qDebug()<<"froit appuyé"<<"\n";
        Deplacement(450,450,4);
    }
    else{//arret
        Deplacement(0,0,0);
    }
}

void OP::on_pause_clicked()
{
     qDebug() << this->pause;
      unsigned char* castData=  (unsigned char*)this->robot.DataToSend.data();

    //Test de la variable pause afin de mettre
    if(this->pause==false){
        ui->pause->setIcon(QIcon(":/images/play.svg"));
        ui->pause->setStyleSheet("background-color: rgb(255, 0, 0);");

        //test de l'exemple
        this->robot.DataToSend[6]=0xF0;
        this->robot.DataToSend[4]=0x00;
        this->robot.DataToSend[2]=0x00;
        this->robot.DataToSend[3]=0x00;
        this->robot.DataToSend[5]=0x00;

        short crc=Crc16(castData+1,6);
        this->robot.DataToSend[7]=(char)crc;
        this->robot.DataToSend[8]=(char)(crc>>8);



        this->pause=true;
        qDebug() << "Dnas le play ";
        qDebug() << this->pause;
    }
    else{
        ui->pause->setIcon(QIcon(":/images/pause.svg"));
        ui->pause->setStyleSheet("background-color: rgb(0,255, 0);");

        this->pause=false;

    }

}

short OP::Crc16(unsigned char *Adresse_tab,unsigned char Taille_max) {

    unsigned int Crc = 0xFFFF;
    unsigned int Polynome = 0xA001;
    unsigned int CptOctet = 0;
    unsigned int CptBit = 0;
    unsigned int Parity= 0;
    Crc = 0xFFFF;
    Polynome = 0xA001;
    for ( CptOctet= 0 ; CptOctet < Taille_max ; CptOctet++)       {
        Crc ^= *( Adresse_tab + CptOctet);
            for ( CptBit = 0; CptBit <= 7 ; CptBit++)             {//amodifier
                Parity= Crc;
                Crc >>= 1;
                if (Parity%2 == true) Crc ^= Polynome;
            }
    }

    return Crc;
}

unsigned int OP::dtobin(unsigned char h)
{
    double n;
   unsigned int b=0;

   for(n=0;n<=7;n++)
   {
       b+=(pow(10,n)*(h%2));
       h/=2;
   }

   return b;
}

void OP::miseAJour(){
    if(this->demarrer){
         emit updateUI(robot.DataReceived);
        int BatVoltage;
        int SpeedFrontLeft;
        int SpeedRearLeft;
        int SpeedFrontRight, SpeedRearRight, IRLeft, IRRight;
        this->BatVoltage=(int)robot.DataReceived[2];
        long odometry=( (((long)robot.DataReceived[8]<<24))+(((long)robot.DataReceived[7]<<16))+(((long)robot.DataReceived[6]<<8))+(((long)robot.DataReceived[5])));


         qDebug()<<"BatVoltage ::-----------------------"<<this->BatVoltage;
         qDebug()<<"BatVoltage ::-----------------------"<<odometry;
        /*
         * BatVoltage=(in)robot.DataReceived[2];
         *
         * int IR=(unsigned
















          */
         //Mise à jour des valeurs recupere par intervalles de 2 s
         int ondo=(int)odometry;
          ui->batterieBar->setValue(this->BatVoltage);
          ui->odometrie->display(ondo);

          ui->energie->display(0);
          ui->ticRight->display(0);
          ui->ticLeft->display(0);


      }
    else{
        ui->batterieBar->setValue(0);


    }


}
void OP::keyPressEvent(QKeyEvent *event){
    qDebug()<<"UPPPPPPPPPP11111111111";
    switch(event->key()){
                qDebug()<<"UPPPPPPPPPP333333333";
                case Qt::Key_Up:
                    qDebug()<<"UPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPUPUPUPUPUPUPUPUPUPUPUPUPUPPUPUPUPUPUPUPU";
                    this->on_avancer_pressed();
                    ui->avancer->setStyleSheet("background-color: rgb(0, 170, 0);");
                    ui->avancer->setStyleSheet("border-radius:15px;");
                break;
                case Qt::Key_Down:
                    this->on_reculer_pressed();
                break;
                case Qt::Key_Left:
                      this->on_gauche_pressed();
                break;
                case Qt::Key_Right:
                        this->on_droite_pressed();
                break;

    }
    qDebug()<<"UPPPPPPPPPP222222222";
}
void OP::keyReleaseEvent(QKeyEvent *event){
    switch(event->key()){
                case Qt::Key_Up:
                    ui->avancer->animateClick(25);
                break;
                case Qt::Key_Down:
                    this->on_reculer_released();
                break;
                case Qt::Key_Left:
                    this->on_gauche_released();
                break;
                case Qt::Key_Right:
                   this->on_droite_released();
                break;

    }
}




void OP::on_gauche_clicked()
{

}

void OP::on_gauche_pressed()
{
    wifibot[1]=true;

}

void OP::on_gauche_released()
{
    wifibot[1]=false;
}

void OP::on_avancer_pressed()
{
    wifibot[0]=true;
}

void OP::on_avancer_released()
{
    wifibot[0]=false;
}


void OP::on_reculer_pressed()
{
    wifibot[2]=true;
}

void OP::on_reculer_released()
{
    wifibot[2]=false;
}

void OP::on_droite_clicked()
{

}

void OP::on_droite_pressed()
{
     wifibot[3]=true;
}

void OP::on_droite_released()
{
    wifibot[3]=false;
}
