#ifndef OP_H
#define OP_H

#include "myrobot.h"
#include <QDialog>
#include "math.h"

#include <QTimer>
#include <QWebEngineView>
#include <QString>
namespace Ui {
class OP;
}

class OP : public QDialog
{
    Q_OBJECT

public:
    explicit OP(QWidget *parent = nullptr);
    ~OP();



private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_avancer_clicked();

    void on_reculer_clicked();

    void on_pause_clicked();



    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);


    short Crc16(unsigned char *Adresse_tab ,unsigned char Taille_max);

     unsigned int dtobin(unsigned char h);

     void miseAJour();

     void on_gauche_clicked();

     void on_gauche_pressed();

     void on_gauche_released();

     void on_avancer_pressed();

     void on_avancer_released();

     void on_reculer_pressed();

     void on_reculer_released();

     void on_droite_clicked();

     void on_droite_pressed();

     void on_droite_released();

     void envoi();

     void Deplacement(unsigned char leftspeed, unsigned char rightspeed, int Direction);

signals:
    void updateUI(const QByteArray Data);

private:
    Ui::OP *ui;
    QString username;
    MyRobot robot;
    //QKeyEvent* event;
    int vitesse=0;
    bool pause=false,demarrer=false;

    int BatVoltage;
    int SpeedFrontLeft;
    int SpeedRearLeft;
    int SpeedFrontRight, SpeedRearRight, IRLeft, IRRight;

    //Tableau de controles : A,R,G,D
    bool* wifibot;
    QWebEngineView* view;
   // QTimer *timer2;
      QNetworkAccessManager *camera;
      QString text;
};

#endif // OP_H
