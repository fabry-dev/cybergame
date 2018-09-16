#include "maintenance.h"
#include "qdebug.h"
#include "qtimer.h"
#include "mainwindow.h"
#include "QFont"
#include "qfontdatabase.h"
#include "button.h"
#include "qtoolbutton.h"

#define CHECK_DELAY 2000


maintenance::maintenance(QWidget *parent, QString filename,QString PATH):QLabel(parent),filename(filename),PATH(PATH)
{

    this->hide();


    background = new QPixmap(filename); //preload image


    this->resize(1080,1920);

    this->setPixmap(*background);

    networkCheck = new QTimer(this);
    connect(networkCheck,SIGNAL(timeout()),this,SLOT(checkNetwork()));




}




void maintenance::start()
{

    this->show();
    networkCheck->start(CHECK_DELAY);

}



void maintenance::checkNetwork(void)
{
    qDebug()<<"checking network...";
    int res =  ((MainWindow*)parent())->requestData("00000");

    if(res!=NETWORK_FAIL)
    {

        networkCheck->stop();
        emit networkIsBack("network");

    }

}




maintenance::~maintenance()
{


}


