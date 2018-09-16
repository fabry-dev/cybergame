#include "game1.h"
#include "qdebug.h"
#include "qtimer.h"
#include "mainwindow.h"
#include "QFont"
#include "qfontdatabase.h"
#include "memorytile.h"
#include "qtoolbutton.h"
#include "QTime"

#define borderX 10
#define borderY 5


#define timeX0 416
#define timeX1 665
#define timeY0 456
#define timeY1 516
#define timeSpace 5



#define deltat 10

#define maxTime 120//120



const int timeToGoHidden[] = {2000,1500,1000};


game1::game1(QWidget *parent, QString filename,QString fontname,QString PATH):QLabel(parent),filename(filename),fontname(fontname),PATH(PATH)
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

    lbltime = new QLabel(this);
    lbltime->setFont(font);
    lbltime->setStyleSheet("QLabel { color : white; }");
    lbltime->setAlignment(Qt::AlignLeft);
    lbltime->resize(timeX1-timeX0,timeY1-timeY0);
    lbltime->move(timeX0+timeSpace,timeY0+borderY);
    font.setPointSizeF(40); //Set font size
    lbltime->setFont(font);

    updTime(0);

    clock = new QTimer(this);
    connect(clock, SIGNAL(timeout()), this, SLOT(tick()));


}

void game1::generateMatrix(int matrixX, int matrixY,int buttonWidth)
{


    int bx,by;
    int bw,bh;
    int offsetx,offsety;
    offsetx = 5;
    offsety = 5;



    int x0,x1,y0,y1;
    for (int i = 0;i<4*6;i++)
    {

        memoryTile *b = new memoryTile(this,0,i,buttonWidth,225,PATH);
        bw = b->width();
        bh = b->height();
        bx = (i%4)*bh-(i%4)*offsetx+((i/4)%2)*(bh/2-offsetx/2);
        by =  (i/4 +1 )*bw - (i/4)*offsety;

        b->move(bx,by);

        connect(b,SIGNAL(clicked(int)),this,SLOT(onClick(int)));
        b->show();
        matrix.push_back(b);

        if (i==0)
        {
            x0 = b->x();
            y0 = b->y()+b->height();

        }
        else if(i==4*6-1)
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

void game1::onClick(int id)
{
    if(selectedTiles.indexOf(id)!=-1)
    {
        selectedTiles.removeOne(id);
    }
    else
    {
        selectedTiles.append(id);
        if(selectedTiles.size()==2)
        {

            if(matrix[selectedTiles[0]]->getType()  == matrix[selectedTiles[1]]->getType())
            {

                for(auto s:selectedTiles)
                {
                    matrix[s]->goWon();
                    wonTiles.append(s);
                    matrix[s]->hide();


                    if(wonTiles.size()>=24)
                    {

                        emit won(1,clockCount);
                        stop();
                    }
                }

                selectedTiles.clear();

            }
            else
            {



            }

        }
        else if(selectedTiles.size()==3)
        {
            for(int i = 0;i<selectedTiles.size()-1;i++)
            {
                matrix[selectedTiles[i]]->goHidden();
            }
            selectedTiles.removeFirst();
            selectedTiles.removeFirst();



        }
    }

}

void game1::pauseTimer()
{

    clock->stop();
}

void game1::unpauseTimer()
{
    clock->start(deltat);
    updTime(0);


}

void game1::tick()
{
    clockCount += deltat/10;

    if(clockCount>maxTime*100)
    {
        emit lost(1,clockCount);
        stop();
    }

    updTime(clockCount);
}

void game1::updTime(ulong nu)
{
    QString time;

    int ms = (nu)%100;
    int sec = (nu/100)%60;
    int min = (nu/6000);


    time = QString("%1:%2:%3").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0')).arg(ms, 2, 10, QChar('0'));

    lbltime->setText(time);


}

void game1::start(int level)
{

    QTime now = QTime::currentTime();
    qsrand(now.msec());
    this->show();

    selectedTiles.clear();
    wonTiles.clear();


    std::vector<int> symbols;

    for(int i = 0;i<12;i++)
    {
        symbols.push_back(i);
        symbols.push_back(i);
    }


    int n;
    for(int i = 0;i<4*6;i++)
    {
        n = qrand()%(symbols.size());

        matrix[i]->setSymbol(symbols[n]);

        symbols.erase(symbols.begin()+n);
    }





    clockCount = 0;

    QTimer::singleShot(timeToGoHidden[level],this,SLOT(goFullHidden()));
    unpauseTimer();
}

void game1::stop()
{
    pauseTimer();
}

void game1::goFullHidden()
{
    qDebug()<<"go full hidden";
    for (auto b:matrix)
    {
        b->goHidden();

    }


}

game1::~game1()
{


}


