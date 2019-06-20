#include "op.h"
#include "ui_op.h"
#include <QMessageBox>
#include <QGraphicsView>
#include <QMediaPlayer>
#include  <QKeyEvent>
#include <QFileDialog>
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

    text="192.168.15.5";
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
    this->view->load(QUrl("http://192.168.1.11:8080/?action=stream"));
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
/*
      qDebug()<<"Datatosend[0]              :--------"<<dtobin(castData[0]);
      qDebug()<<"Datatosend[1]              :--------"<<dtobin(castData[1]);
      qDebug()<<"Datatosend[2]              :--------"<<dtobin(castData[2]);
      qDebug()<<"Datatosend[3]              :--------"<<dtobin(castData[3]);
      qDebug()<<"Datatosend[4]              :--------"<<dtobin(castData[4]);
      qDebug()<<"Datatosend[5]              :--------"<<dtobin(castData[5]);
      qDebug()<<"Datatosend[6]              :--------"<<dtobin(castData[6]);
       qDebug()<<"Datatosend[7]              :--------"<<dtobin(castData[7]);
      qDebug()<<"Datatosend[8]              :--------"<<dtobin(castData[8]); */
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
    QPixmap screenshot =QPixmap();
    screenshot = QPixmap::grabWidget(this->view);
    QString format = "png";
    QString initialPath = QDir::currentPath() + tr("/untitled.") + format;

        QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                   initialPath,
                                   tr("%1 Files (*.%2);;All Files (*)")
                                   .arg(format.toUpper())
                                   .arg(format));
        if (!fileName.isEmpty()){

            screenshot.save(fileName, format.toUtf8());

        }else{
            // On déclenche un audio quand ca marche pas

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
        int vitesseAvD;
        int vitesseAvG;
        int vitesseArD;
        int vitesseArG;
        int irAvG;
        int irAvD;
        int irArG;
        int irArD;
        int odoAvG;
        int odoAvD;
        int odoArG;
        int odoArD;
        /*int SpeedFrontRight, SpeedRearRight, IRLeft, IRRight;
        this->BatVoltage=(int)robot.DataReceived[2];
        long odometry=( (((long)robot.DataReceived[8]<<24))+(((long)robot.DataReceived[7]<<16))+(((long)robot.DataReceived[6]<<8))+(((long)robot.DataReceived[5])));
        */
         this->BatVoltage=(int)robot.DataReceived[2];
        vitesseAvD=(int)robot.DataReceived[0];
        vitesseArD=(int)robot.DataReceived[1];

        vitesseAvG=(int)robot.DataReceived[9];
        vitesseArG=(int)robot.DataReceived[10];

        irAvD=(int)robot.DataReceived[3];
        irAvG=(int)robot.DataReceived[4];

        irArD=(int)robot.DataReceived[11];
        irArG=(int)robot.DataReceived[12];

        odoAvD=(float)robot.DataReceived[5];
        odoAvG=(float)robot.DataReceived[6];

        odoArD=(float)robot.DataReceived[7];
        odoArG=(float)robot.DataReceived[8];




         qDebug()<<"BatVoltage ::-----------------------"<<this->BatVoltage;
        // qDebug()<<"BatVoltage ::-----------------------"<<odometry;
        /*
         * BatVoltage=(in)robot.DataReceived[2];
         *
         * int IR=(unsigned
          */
         //Mise à jour des valeurs recupere par intervalles de 2 s
       // BatVoltage=(int)robot.DataReceived[2];
          ui->batterieBar->setValue(this->BatVoltage);
          ui->vitAvd->display(vitesseAvD);
          ui->vitAvg->display(vitesseAvG);
          ui->vitArd->display(vitesseArD);
          ui->vitArg->display(vitesseArG);

          ui->irAvd->display(irAvD);
          ui->irAvg->display(irAvG);
          ui->irArd->display(irArD);
          ui->irArg->display(irArG);

          ui->odoAvd->display(odoAvD);
          ui->odoAvg->display(odoAvG);
          ui->odoArd->display(odoArD);
          ui->odoArg->display(odoArG);
      }
    else{
        ui->batterieBar->setValue(0);


    }


}
void OP::keyPressEvent(QKeyEvent *event){
    //qDebug()<<"UPPPPPPPPPP11111111111";
    switch(event->key()){






// Controle du deplacement de la camera
    case Qt::Key_K:
        camera->get(QNetworkRequest(QUrl("http://"+text+":8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200")));
        this->view->show();
        break;
    case Qt::Key_L:
        camera->get(QNetworkRequest(QUrl("http://"+text+":8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200")));
        this->view->show();
        break;
    case Qt::Key_O:
        camera->get(QNetworkRequest(QUrl("http://"+text+":8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200")));
        this->view->show();
        break;
    case Qt::Key_M:
        camera->get(QNetworkRequest(QUrl("http://"+text+":8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200")));
        this->view->show();
        break;







    case Qt::Key_Z:

        qDebug()<<"UPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP";
          ui->avancer->animateClick(25);
            this->on_avancer_pressed();
          ui->avancer->setStyleSheet("background-color: rgb(0, 250, 0);");
          ui->avancer->setStyleSheet("border-radius:15px;");

        break;
    case Qt::Key_Q:
        qDebug()<<"DROITEEEEEEEEEEEEEEEEEEEEEEEEEEEEE";
          ui->droite->animateClick(25);
            this->on_droite_pressed();
          ui->droite->setStyleSheet("background-color: rgb(0, 250, 0);");
          ui->droite->setStyleSheet("border-radius:15px;");


        break;
    case Qt::Key_S:
        qDebug()<<"DOWNNNNNNNNNNNNNNNNNNNNN";
          ui->reculer->animateClick(25);
            this->on_reculer_clicked();
          ui->reculer->setStyleSheet("background-color: rgb(0, 250, 0);");
          ui->reculer->setStyleSheet("border-radius:15px;");

        break;
    case Qt::Key_D:
        qDebug()<<"LEFTTTTTTTTTTTTTTTTTTTTTTTTTT";
          ui->droite->animateClick(25);
            this->on_gauche_pressed();
          ui->droite->setStyleSheet("background-color: rgb(0, 250, 0);");
          ui->droite->setStyleSheet("border-radius:15px;");


        break;


    }

}
void OP::keyReleaseEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Z:

        qDebug()<<"UPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP";
          ui->avancer->animateClick(25);
            this->on_avancer_pressed();
          ui->avancer->setStyleSheet("background-color: rgb(0, 0, 0);");
          ui->avancer->setStyleSheet("border-radius:0px;");

        break;
    case Qt::Key_Q:
        qDebug()<<"DROITEEEEEEEEEEEEEEEEEEEEEEEEEEEEE";
          ui->gauche->animateClick(25);
          ui->gauche->setStyleSheet("background-color: rgb(0, 0, 0);");
          ui->gauche->setStyleSheet("border-radius:0px;");



        break;
    case Qt::Key_S:
        qDebug()<<"DOWNNNNNNNNNNNNNNNNNNNNN";
          ui->reculer->animateClick(25);
          ui->reculer->setStyleSheet("background-color: rgb(0, 0, 0);");
          ui->reculer->setStyleSheet("border-radius:0px;");


        break;
    case Qt::Key_D:
        qDebug()<<"LEFTTTTTTTTTTTTTTTTTTTTTTTTTT";
        ui->droite->animateClick(25);
        ui->droite->setStyleSheet("background-color: rgb(0, 0, 0);");
        ui->droite->setStyleSheet("border-radius:0px;");



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
   // qDebug()<<"UPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPUPUPUPUPUPUPUPUPUPUPUPUPUPPUPUPUPUPUPUPU";
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
