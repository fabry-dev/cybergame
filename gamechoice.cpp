#include "gamechoice.h"
#include "qdebug.h"
#include "qtimer.h"
#include "mainwindow.h"
#include "QFont"
#include "qfontdatabase.h"
#include "button.h"
#include "qtoolbutton.h"






gamechoice::gamechoice(QWidget *parent, QString filename,QString fontname,QString PATH):QLabel(parent),filename(filename),fontname(fontname),PATH(PATH)
{

    this->hide();

    int id = QFontDatabase::addApplicationFont(fontname);
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    font = QFont(family,15);



    background = new QPixmap(filename); //preload image
    this->resize(1080,1920);

    this->setPixmap(*background);

    generateMatrix(width()/2,1000,400);

    home = new button(this,"",0,80,10,"home.png","home.png",true,PATH);
    connect(home,SIGNAL(clicked(int)),this,SLOT(timeout()));
    connect(home,SIGNAL(clicked(int)),this,SLOT(stopGoHomeTimer()));
    home->move((width()-home->width())/2,1810);
    home->show();


    goHomeTimer = new QTimer;
    goHomeTimer->setSingleShot(true);
    connect(goHomeTimer,SIGNAL(timeout()),this,SLOT(timeout()));

}


void gamechoice::generateMatrix(int matrixX, int matrixY,int buttonWidth)
{


    int bx,by;
    int bw,bh;
    int offsetx,offsety;
    offsetx = 0;
    offsety = 15;


    QString gameName;


    int x0,x1,y0,y1;
    for (int i = 0;i<2;i++)
    {
        if (i==0)
            gameName = "memory";
        else
            gameName = "defence";

        button *b = new button(this,"",i+1,buttonWidth,85,gameName+"GameRed.png",gameName+"GameGreen.png",false,PATH);
        bw = b->width();
        bh = b->height();

        bx = (i)*bh*b->ratio/(2)+(i)*offsetx;
        by =  (i +1 )*bw - (i)*offsety;

        b->move(bx,by);

        connect(b,SIGNAL(clicked(int)),this,SLOT(starter(int)));



        b->show();
        matrix.push_back(b);

        if (i==0)
        {
            x0 = b->x();
            y0 = b->y()+b->height();
        }
        else if(i==1)
        {

            x1 = b->x()+b->width();
            y1 = b->y();
        }
    }

    for (auto b:matrix)
        b->move(b->x()+0-(x1+x0)/2+matrixX,b->y()+matrixY-(y1+y0)/2);

    /* QLabel *lbl = new QLabel(this);
       lbl->setStyleSheet("border: 1px solid red");
       lbl->setGeometry(x0,y1,x1-x0,y0-y1);*/


}


void gamechoice::starter(int gameid)
{
    stopGoHomeTimer();
    emit startGame(gameid);
}


void gamechoice::timeout()
{

   ( (MainWindow*)(this->parent()))->goHome("game choice");
}


void gamechoice::stopGoHomeTimer(void)
{
    goHomeTimer->stop();
}


void gamechoice::completeMission()
{
    emit missionCompleted("mission completed");

}

void gamechoice::start()
{
/*
    if(((((MainWindow*)parent())->getUserData().attempt1 >=3)&&(((MainWindow*)parent())->getUserData().attempt2 >=3))||true)
    {
     this->show();
        QTimer::singleShot(2000,this,SLOT(completeMission()));




    }

*/
    goHomeTimer->start(timeToGoHome);



    if(((MainWindow*)parent())->getUserData().attempt1 >=3)
        matrix[0]->hide();
    else
        matrix[0]->show();
    if(((MainWindow*)parent())->getUserData().attempt2 >=3)
        matrix[1]->hide();
    else
        matrix[1]->show();

 this->show();




}


gamechoice::~gamechoice()
{


}


