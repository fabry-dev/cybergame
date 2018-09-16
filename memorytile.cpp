#include "memorytile.h"
#include "algorithm"
#include "qdebug.h"
#include "qtimer.h"
#include "mainwindow.h"
#include "QFont"
#include "qfontdatabase.h"
#include "qpalette.h"
#include "qtextdocument.h"
#include "qstatictext.h"


#define offPic "greenButton.png"
#define fontName "Gotham-Medium.otf"



#define HIDDEN 0
#define UNCHECKED 1
#define CHECKED 2
#define WON 3


QStringList symbols = QStringList() << "1" << "2" << "3" << "4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12";

memoryTile::memoryTile(QWidget* parent , int type, int id, int w, int fontsize,QString PATH)
    : QLabel(parent),type(type),id(id),w(w),PATH(PATH)
{

    int fontid = QFontDatabase::addApplicationFont(PATH+fontName);
    QString family = QFontDatabase::applicationFontFamilies(fontid).at(0);
    font = QFont(family,fontsize);

    offPix = new QImage;
    onPix = new QImage;
    hiddenPix = new QImage(PATH+offPic);
    setSymbol(type);
    state = UNCHECKED;
}


void memoryTile::setWidth(int w)
{
    int h = offPix->height()*w/offPix->width();
    this->resize(w,h);
}


void memoryTile::setSymbol(int symbol)
{
    QString s;
    state = UNCHECKED;
    type = symbol;
    s = PATH+"memoryRed"+QString::number(symbol+1)+".png";
    offPix->load(s);
    s = PATH+"memoryGrey"+QString::number(symbol+1)+".png";
    onPix->load(s);
    ratio = (double)(offPix->width())/(offPix->height());
    setWidth(w);
    drawState();
    show();
}


bool memoryTile::isSelected()
{
    return (state==CHECKED);
}


void memoryTile::drawState()
{
    QPixmap pix;

    switch(state)
    {
    case CHECKED:

        pix = QPixmap::fromImage((*onPix).scaled(width(),height()));
        break;

    case UNCHECKED:

        pix = QPixmap::fromImage((*offPix).scaled(width(),height()));
        break;

    case HIDDEN:

        pix = QPixmap::fromImage((*hiddenPix).scaled(width(),height()));
        break;

    case WON:

        pix = QPixmap::fromImage((*offPix).scaled(width(),height()));
        break;
    }



    setPixmap(pix);


}


void memoryTile::changeState()
{
    if(state == HIDDEN)
        state = CHECKED;
    //else if(state == CHECKED)
    //    state = HIDDEN;

    drawState();

}


void memoryTile::goHidden(void)
{
    state = HIDDEN;
    drawState();
}


void memoryTile::goWon(void)
{
    state = WON;
    drawState();
}


int memoryTile::getType()
{
    return type;
}


void memoryTile::mousePressEvent( QMouseEvent* ev )
{
    QImage image(pixmap()->toImage());
    QColor color(image.pixel(ev->pos()));


    if(state != HIDDEN)
        return;


    if((color.black()==255)&(color.hslHue()==-1)&(color.hslSaturation()==0))//transparent area
        return;


    changeState();

    emit clicked(id);

}

void memoryTile::mouseReleaseEvent(QMouseEvent * ev)
{
    //drawState(false);
    //emit released(id);
}


memoryTile::~memoryTile()
{


}

