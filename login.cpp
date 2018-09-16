#include "login.h"
#include "qdebug.h"
#include "qtimer.h"
#include "mainwindow.h"
#include "QFont"
#include "qfontdatabase.h"
#include "button.h"
#include "qtoolbutton.h"

#define loginLength 5


#define borderX 10
#define borderY 5
#define textX0 217
#define textX1 857
#define textY0 759
#define textY1 861


#define validateX0 206
#define validateY0 1375
#define validateX1 458
#define validateY1 1443

#define deleteX0 627
#define deleteY0 1375
#define deleteX1 885
#define deleteY1 1442

#define COMPLETED "MISSION COMPLETED"
#define DENIED "ACCESS DENIED"
#define GRANTED "ACCESS GRANTED"



login::login(QWidget *parent, QString filename,QString fontname,QString PATH):QLabel(parent),filename(filename),fontname(fontname),PATH(PATH)
{
qDebug()<<"path "<<PATH;
    this->hide();
    //this->setStyleSheet("border: 1px solid black");
    int id = QFontDatabase::addApplicationFont(fontname);
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    font = QFont(family,10);



    background = new QPixmap(filename); //preload image
    this->resize(1080,1920);

    this->setPixmap(*background);

    generateMatrix(width()/2,1100,130);

    loginValue = "";

    txt = new QLabel(this);
    txt->setText("");
    txt->setFont(font);
    txt->setStyleSheet("QLabel { color : white; }");
    //txt->setFont(font);
    txt->setAlignment(Qt::AlignCenter);
    txt->resize(textX1-textX0,textY1-textY0);
    txt->move(textX0,textY0+borderY);
    lock = false;
}



void login::generateMatrix(int matrixX, int matrixY,int buttonWidth)
{


    int bx,by;
    int bw,bh;
    int offsetx,offsety;
    offsetx = 5;
    offsety = 5;


    int num;
    int x0,x1,y0,y1;
    for (int i = 0;i<10;i++)
    {
        num = i + 1;
        if (num == 10)
            num = 0;


        button *b = new button(this,QString::number(num),num,buttonWidth,90,"","",true,PATH);
        bw = b->width();
        bh = b->height();
        bx = (i%5)*bh-(i%5)*offsetx+(i/5)*(bh/2-offsetx/2);
        by =  (i/5 +1 )*bw - (i/5)*offsety;

        b->move(bx,by);

        connect(b,SIGNAL(clicked(int)),this,SLOT(onClick(int)));
        b->show();
        matrix.push_back(b);

        if (i==0)
        {
            x0 = b->x();
            y0 = b->y()+b->height();

        }
        else if(i==9)
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

void login::updLogin(int nu)
{


    if (nu<0)
        loginValue = "";




    if (loginValue.size()>=loginLength)
        return;


    if (nu>=0)
        loginValue.append(QString::number(nu));

    QFontMetrics metrics(font);
    QSize size = metrics.size(0, loginValue); //Get size of text
    float factorw = (textX1-textX0-2*borderX) / (float)size.width(); //Get the width factor
    float factorh = (textY1-textY0) / (float)size.height(); //Get the height factor
    float factor = qMin(factorw, factorh); //To fit contents in the screen select as factor
    //the minimum factor between width and height

    font.setPointSizeF(font.pointSizeF() * factor); //Set font size

    txt->setFont(font);
    txt->setText(loginValue);

}

void login::mousePressEvent( QMouseEvent* ev )
{
    QPoint pt = ev->pos();

    if(lock)
        return;

    if((pt.x()>validateX0)&&(pt.x()<validateX1)&&(pt.y()>validateY0)&(pt.y()<validateY1))
    {

        if (loginValue.size()==loginLength)
            validateLogin();


    }
    else if ((pt.x()>deleteX0)&&(pt.x()<deleteX1)&&(pt.y()>deleteY0)&(pt.y()<deleteY1))
    {
        updLogin(-1);
    }

}

void login::validateLogin()
{

    connectionAttempts++;

    int res =  ((MainWindow*)parent())->requestData(loginValue);



    if(res==NETWORK_OK)
    {

        if((((MainWindow*)parent())->getUserData().attempt1>=3)&&(((MainWindow*)parent())->getUserData().attempt2>=3))
        {
            //game completed
            loginValue = "";
            QFontMetrics metrics(font);
            QSize size = metrics.size(0, COMPLETED); //Get size of text
            float factorw = (textX1-textX0-2*borderX) / (float)size.width(); //Get the width factor
            float factorh = (textY1-textY0) / (float)size.height(); //Get the height factor
            float factor = qMin(factorw, factorh); //To fit contents in the screen select as factor
            //the minimum factor between width and height

            font.setPointSizeF(font.pointSizeF() * factor); //Set font size

            txt->setFont(font);
            txt->setText(COMPLETED);


        }

        else
        {
            lock = true;//lock all controls.
            QTimer::singleShot(1500,this,SLOT(preSend()));
            QFontMetrics metrics(font);
            QSize size = metrics.size(0, GRANTED); //Get size of text
            float factorw = (textX1-textX0-2*borderX) / (float)size.width(); //Get the width factor
            float factorh = (textY1-textY0) / (float)size.height(); //Get the height factor
            float factor = qMin(factorw, factorh); //To fit contents in the screen select as factor
            //the minimum factor between width and height

            font.setPointSizeF(font.pointSizeF() * factor); //Set font size

            txt->setFont(font);


            txt->setText(GRANTED);
        }

    }
    else if(res == NETWORK_NOUSER)
    {
        loginValue = "";
        lock = false;//unlock all controls.
        QFontMetrics metrics(font);
        QSize size = metrics.size(0, DENIED); //Get size of text
        float factorw = (textX1-textX0-2*borderX) / (float)size.width(); //Get the width factor
        float factorh = (textY1-textY0) / (float)size.height(); //Get the height factor
        float factor = qMin(factorw, factorh); //To fit contents in the screen select as factor
        //the minimum factor between width and height

        font.setPointSizeF(font.pointSizeF() * factor); //Set font size

        txt->setFont(font);
        txt->setText(DENIED);

    }
    else
    {
        if(connectionAttempts>maxNetworkAttempt)
            emit networkError();
        else
        {
            lock = true;//lock all controls.
            QTimer::singleShot(500,this,SLOT(validateLogin()));

        qDebug()<<"network error #"<<connectionAttempts;
            }
    }

}

void login::preSend()
{
    emit sendLogin();

}

void login::start()
{
    connectionAttempts = 0;
    lock = false;
    updLogin(-1);
    this->show();


}













void login::onClick(int buttonid)
{

    //qDebug()<<"button #"<<buttonid<<" clicked.";
    updLogin(buttonid);
}


login::~login()
{


}


