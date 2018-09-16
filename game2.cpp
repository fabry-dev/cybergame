#include "game2.h"
#include "qdebug.h"
#include "qtimer.h"
#include "mainwindow.h"
#include "QFont"
#include "qfontdatabase.h"
#include "button.h"
#include "qtoolbutton.h"



//#define BADGUYTIMEON 350
#define BADGUYTIMEGAP 1000



//const int badGuyTimeOn[] = {350,300,250};


#define borderX 10
#define borderY 5


#define timeX0 170
#define timeX1 424
#define timeY0 456
#define timeY1 516
#define timeSpace 5


#define scoreX0 662
#define scoreX1 910
#define scoreY0 456
#define scoreY1 516

#define deltat 10



#define maxTime 90

game2::game2(QWidget *parent, QString filename,QString fontname,QString PATH):QLabel(parent),filename(filename),fontname(fontname),PATH(PATH)
{


    this->hide();
    int id = QFontDatabase::addApplicationFont(fontname);
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    font = QFont(family,15);
    background = new QPixmap(filename); //preload image
    this->resize(1080,1920);

    this->setPixmap(*background);

    generateMatrix(width()/2,1000,120);

    clockCount = 0;
    score = 0;
    loadParameters();

    lblscore = new QLabel(this);
    lblscore->setFont(font);
    lblscore->setStyleSheet("QLabel { color : white; }");
    lblscore->setAlignment(Qt::AlignCenter);
    lblscore->resize(scoreX1-scoreX0,scoreY1-scoreY0);
    lblscore->move(scoreX0,scoreY0+borderY);


    lbltime = new QLabel(this);
    lbltime->setFont(font);
    lbltime->setStyleSheet("QLabel { color : white; }");
    lbltime->setAlignment(Qt::AlignLeft);
    lbltime->resize(timeX1-timeX0,timeY1-timeY0);
    lbltime->move(timeX0+timeSpace,timeY0+borderY);
    font.setPointSizeF(40); //Set font size
    lbltime->setFont(font);

    updScore(0);
    updTime(0);

    clock = new QTimer(this);
    connect(clock, SIGNAL(timeout()), this, SLOT(tick()));
}


void game2::pauseTimer()
{
    clock->stop();
}


void game2::unpauseTimer()
{
    clock->start(deltat);
}


void game2::generateMatrix(int matrixX, int matrixY,int buttonWidth)
{
    int bx,by;
    int bw,bh;
    int offsetx,offsety;

    offsetx = -15;
    offsety = 25;


    const int n[7] = {1,2,3,4,3,2,1};
    int num = 0;

    int x0,x1,y0,y1;


    for (int j=0;j<7;j++)
    {

        for (int i=0;i<n[j];i++)
        {
            button *b = new button(this,"",num,buttonWidth,225,"","",true,PATH);
            bw = b->width();
            bh = b->height();
            bx = (i%4)*bw-(i%4)*offsetx+((i/4)%2)*(bw/2-offsetx/2) -(n[j]-1)*(bw/2-offsetx/2);
            by =  (j +1 )*bh - j*offsety;



            b->move(bx,by);

            connect(b,SIGNAL(clicked(int)),this,SLOT(onClick(int)));
            b->show();
            matrix.push_back(b);


            if (num==0)
            {
                x0 = b->x();
                y0 = b->y()+b->height();

            }
            else if(num==15)
            {

                x1 = b->x()+b->width();
                y1 = b->y();

            }

            num++;
        }

    }







    // }

    for (auto b:matrix)
        b->move(b->x()+0-(x1+x0)/2+matrixX,b->y()+matrixY-(y1+y0)/2);

    /* QLabel *lbl = new QLabel(this);
       lbl->setStyleSheet("border: 1px solid red");
       lbl->setGeometry(x0,y1,x1-x0,y0-y1);*/


}




void game2::nextStep()
{
    if(!gameActive)
        return;

    if(badGuy)
    {
        //Already showing a bad guy
        badGuy = false;
        matrix[badPos]->stopBadGuy();
        QTimer::singleShot(BADGUYTIMEGAP,this,SLOT(nextStep()));
    }
    else
    {
        badGuy = true;
        int r = badPos;
        while(r==badPos)
            badPos = qrand()%16;
        // qDebug()<<"bad guy "<<badPos;

        matrix[badPos]->goBadGuy();


        QTimer::singleShot(badGuyTimeOn[level-1],this,SLOT(nextStep()));

    }

}


void game2::onClick(int id)
{
    if(id==-1)
    {
        score++;
        updScore(score);
    }

}


void game2::updScore(int nu)
{
    QString score;
    score = QString::number(nu);

    QFontMetrics metrics(font);
    QSize size = metrics.size(0, score); //Get size of text
    float factorw = (scoreX1-scoreX0-2*borderX) / (float)size.width(); //Get the width factor
    float factorh = (scoreY1-scoreY0) / (float)size.height(); //Get the height factor
    float factor = qMin(factorw, factorh); //To fit contents in the screen select as factor
    //the minimum factor between width and height

    font.setPointSizeF(font.pointSizeF() * factor); //Set font size

    lblscore->setFont(font);
    lblscore->setText(score);
}


void game2::tick()
{
    clockCount += deltat/10;
    updTime(clockCount);

    if(clockCount>100*maxTime)
    {
        emit won(2,score);
        stop();
    }
}


void game2::updTime(ulong nu)
{
    QString time;

    int ms = (nu)%100;
    int sec = (nu/100)%60;
    int min = (nu/6000);


    time = QString("%1:%2:%3").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0')).arg(ms, 2, 10, QChar('0'));

    lbltime->setText(time);
}


void game2::start(int nulevel)
{
    level = nulevel;
    this->show();
    for (auto b:matrix)
        b->stopBadGuy();

    score = 0;
    updScore(0);
    clockCount = 0;
    badGuy = false;
    gameActive = true;
    badPos = -1;
    QTimer::singleShot(1000,this,SLOT(nextStep()));
    unpauseTimer();
}


void game2::stop()
{
    gameActive = false;
    pauseTimer();
}

void game2::loadParameters()
{
    badGuyTimeOn.push_back(350);
    badGuyTimeOn.push_back(325);
    badGuyTimeOn.push_back(300);


    QFile tfile(PATH+"game2.cfg");
    if(!tfile.open(QIODevice::ReadOnly)) {
        qDebug()<<"could not load cfg file";

    }

    QTextStream in(&tfile);
    QStringList content;

    while(!in.atEnd()) {
        content.append(in.readLine());

    }

    tfile.close();


    if(content.size()<3)
    {



        qDebug()<<"cfg file issue";

    }
    else
    {
        badGuyTimeOn[0] = content[0].toInt();
        badGuyTimeOn[1] = content[1].toInt();
        badGuyTimeOn[2] = content[2].toInt();

    }


    for(auto i:badGuyTimeOn)
        qDebug()<<i;


}


game2::~game2()
{


}


