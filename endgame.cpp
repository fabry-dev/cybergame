#include "endgame.h"
#include "qdebug.h"
#include "qtimer.h"
#include "mainwindow.h"
#include "QFont"
#include "qfontdatabase.h"
#include "button.h"
#include "qtoolbutton.h"


endGame::endGame(QWidget *parent, QString filename, QString fontname,QString PATH):QLabel(parent),filename(filename),fontname(fontname),PATH(PATH)
{


    this->hide();


    int id = QFontDatabase::addApplicationFont(fontname);
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    font = QFont(family,15);

    background = new QPixmap(filename); //preload image

    this->resize(1080,1920);

    font = QFont(family,250);
    home = new button(this,"",0,80,10,"home.png","home.png",true,PATH);
    connect(home,SIGNAL(clicked(int)),this,SLOT(done()));
    home->move((width()-home->width())/2,1810);
    home->show();

    goHomeTimer = new QTimer;
    goHomeTimer->setSingleShot(true);
    connect(goHomeTimer,SIGNAL(timeout()),this,SLOT(timeout()));


    generateMatrix(width()/2,1500,200);




}
void endGame::timeout()
{

    ( (MainWindow*)(this->parent()))->goHome("end game");
}




void endGame::start(int nugameId, int nuscore)
{

    gameId = nugameId;

    this->show();


    if((((MainWindow*)parent())->getUserData().attempt1 >=2)&&(gameId==1))
    {
        background->load(PATH+"finalBG.png");

    }
    else if((((MainWindow*)parent())->getUserData().attempt2 >=2)&&(gameId==2))
    {

        background->load(PATH+"finalBG.png");
    }

    else
        background->load(PATH+"completedBG.png");


    this->setPixmap(*background);
    matrix[0]->hide();
    matrix[1]->hide();


    ((MainWindow*)parent())->newAttempt(nugameId);

    score = ((MainWindow*)parent())->getScore(gameId);




    if(score == 0)
        score = nuscore;
    else
    {
        if(gameId == 1) //memory: the smaller the score, the better
        {
            if((nuscore<score))//new score smaller than old score
                score = nuscore; //update
        }
        else//defence the larger the score, the better
        {
            if(nuscore>score)//new score bigger than old score
                score = nuscore; //update
        }
    }


    ((MainWindow*)parent())->setScore(gameId,score);


    connectionAttempts = 0;


    updateScore();//send data over network






}



void endGame::updateScore()
{
    qDebug()<<"updating game"<<gameId<<" score: ";
    qDebug()<<"nu score: "<<score;
    qDebug()<<"nu attempt: "<<((MainWindow*)parent())->getAttempt(gameId);


    connectionAttempts++;

    int res =  ((MainWindow*)parent())->postData(((MainWindow*)parent())->getId(),gameId,score,((MainWindow*)parent())->getAttempt(gameId));
    if(res==NETWORK_OK)
    {
        doneUpdatingScore();
    }
    else
    {
        if(connectionAttempts>maxNetworkAttempt)
        {
            emit networkError();
        }
        else
        {
            QTimer::singleShot(500,this,SLOT(updateScore()));
            qDebug()<<"network error #"<<connectionAttempts;
        }
    }

}


void endGame::doneUpdatingScore()
{

    if((((MainWindow*)parent())->getUserData().attempt1 >=3)&&(((MainWindow*)parent())->getUserData().attempt2 >=3))
    {

        QTimer::singleShot(3000,this,SLOT(realDone()));

    }
    else if((((MainWindow*)parent())->getUserData().attempt1 >=3)&&(gameId==1))
    {

        QTimer::singleShot(3000,this,SLOT(done()));
    }
    else if((((MainWindow*)parent())->getUserData().attempt2 >=3)&&(gameId==2))
    {

        QTimer::singleShot(3000,this,SLOT(done()));
    }

    else
    {
        matrix[0]->show();
        matrix[1]->show();
    }


    goHomeTimer->start(timeToGoHome);
}


void endGame::realDone()
{

    stopGoHomeTimer();
    ((MainWindow*)parent())->goHome("end game finished");

}

void endGame::done()
{
    stopGoHomeTimer();
    emit doneShowingScore(gameId);

}


void endGame::stopGoHomeTimer()
{

    goHomeTimer->stop();
    qDebug()<<"go home timer end game stopped";
}


void endGame::replay()
{
    goHomeTimer->stop();
    emit startGame(gameId);

}


void endGame::generateMatrix(int matrixX, int matrixY,int buttonWidth)
{


    int bx,by;
    int bw,bh;
    int offsetx,offsety;
    offsetx = 0;
    offsety = 15;


    QString buttonName;



    int x0,x1,y0,y1;
    for (int i = 0;i<2;i++)
    {
        if (i==0)
            buttonName = "yes";
        else
            buttonName = "no";

        button *b = new button(this,"",i+1,buttonWidth,85,buttonName+"On.png",buttonName+"Off.png",false,PATH);
        bw = b->width();
        bh = b->height();

        bx = (i)*bh*b->ratio/(2)+(i)*offsetx;
        by =  (i +1 )*bw - (i)*offsety;

        b->move(bx,by);





        b->show();
        matrix.push_back(b);

        if (i==0)
        {
            connect(b,SIGNAL(clicked(int)),this,SLOT(replay()));
            x0 = b->x();
            y0 = b->y()+b->height();

        }
        else if(i==1)
        {
            connect(b,SIGNAL(clicked(int)),this,SLOT(done()));
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


endGame::~endGame()
{


}


