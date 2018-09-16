#include "countdown.h"
#include "qdebug.h"
#include "qtimer.h"
#include "mainwindow.h"
#include "QFont"
#include "qfontdatabase.h"
#include "button.h"
#include "qtoolbutton.h"


#define WIDTH 400
#define framePic1 PATH+"count1.png"
#define framePic2 PATH+"count2.png"
#define framePic3 PATH+"count3.png"

countDown::countDown(QWidget *parent, QString filename,QString fontname,QString PATH):QLabel(parent),filename(filename),fontname(fontname),PATH(PATH)
{


    this->hide();

    int id = QFontDatabase::addApplicationFont(fontname);
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    font = QFont(family,15);



    background = new QPixmap(filename); //preload image
    this->resize(1080,1920);

    this->setPixmap(*background);

    frame = new QLabel(this);

    font = QFont(family,250);






    framePix1 = new QImage(framePic1); //preload image
    framePix2 = new QImage(framePic2); //preload image
    framePix3 = new QImage(framePic3); //preload image





    //frame->setStyleSheet("border: 1px solid red");




}


void countDown::start(int gameId)
{

    count = 6;
    this->show();
    game = gameId;
    showNextDigit();
}




void countDown::showNextDigit()
{
QPixmap px;





    if(count>1)
    {
        if (count%2)
        {
            QTimer::singleShot(100,this,SLOT(showNextDigit()));
            frame->hide();
        }
        else
        {
            frame->show();
            QTimer::singleShot(900,this,SLOT(showNextDigit()));
            //p1.drawText(frame->rect(), Qt::AlignCenter|Qt::AlignHCenter, QString::number(count/2));

            if(count == 6)
                px = QPixmap::fromImage((*framePix3).scaledToWidth(WIDTH));
            else if(count == 4)
                px = QPixmap::fromImage((*framePix2).scaledToWidth(WIDTH));
            else
                px = QPixmap::fromImage((*framePix1).scaledToWidth(WIDTH));

            frame->resize(px.width(),px.height());




        }
    }
    else
    {
        done(game);
        frame->hide();
        //QTimer::singleShot(1000,this,SIGNAL(done()));
    }

    count --;


    frame->setPixmap(px);
    frame->move((width()-frame->width())/2,(height()-frame->height())/2);




}







countDown::~countDown()
{


}


