#include "rules.h"
#include "qdebug.h"
#include "qtimer.h"
#include "mainwindow.h"
#include "QFont"
#include "qfontdatabase.h"
#include "button.h"
#include "qtoolbutton.h"


#define game1RulesBG1 PATH+"gameRules1.png"
#define game1RulesBG2 PATH+"gameRules2.png"

#define startX0 437
#define startY0 1638
#define startX1 644
#define startY1 1704



gamerules::gamerules(QWidget *parent,QString PATH):QLabel(parent),PATH(PATH)
{


    this->hide();

    background1 = new QPixmap(game1RulesBG1); //preload image
    background2 = new QPixmap(game1RulesBG2); //preload image


    this->resize(1080,1920);



    goHomeTimer = new QTimer;
    goHomeTimer->setSingleShot(true);
    connect(goHomeTimer,SIGNAL(timeout()),this,SLOT(timeout()));

    arrow = new button(this,"",0,60,10,"back.png","back.png",true,PATH);
    connect(arrow,SIGNAL(clicked(int)),this,SLOT(done()));
    arrow->move(50,1810);
    arrow->show();


}


void gamerules::timeout()
{

   ( (MainWindow*)(this->parent()))->goHome("game rules");
}


void gamerules::done()
{

    stopGoHomeTimer();
    emit goBackToChoice();

}


void gamerules::stopGoHomeTimer(void)
{

    goHomeTimer->stop();

}


void gamerules::start(int nuGameId)
{
    gameId = nuGameId;



    if(gameId == 1)
        this->setPixmap(*background1);
    else if (gameId == 2)
        this->setPixmap(*background2);


    goHomeTimer->start(timeToGoHome);

    this->show();

}


void gamerules::mousePressEvent( QMouseEvent* ev )
{
    QPoint pt = ev->pos();


    if((pt.x()>startX0)&&(pt.x()<startX1)&&(pt.y()>startY0)&(pt.y()<startY1))
    {

        stopGoHomeTimer();
        emit startGame(gameId);

    }


}



gamerules::~gamerules()
{


}


