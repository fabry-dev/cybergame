#include "button.h"
#include "algorithm"
#include "button.h"
#include "qdebug.h"
#include "qtimer.h"
#include "mainwindow.h"
#include "QFont"
#include "qfontdatabase.h"
#include "qpalette.h"
#include "qtextdocument.h"
#include "qstatictext.h"



#define badPic "badguy.png"
#define onPic "redButton.png"
#define offPic "greenButton.png"
#define fontName "Gotham-Medium.otf"

button::button(QWidget* parent , QString txt, int id, int w, int fontsize, QString on, QString off,bool ignoreEmptySpace,QString PATH)
    : QLabel(parent),id(id),ignoreEmptySpace(ignoreEmptySpace)
{



    //setTextFormat(Qt::RichText);
    setText(txt);


    int fontid = QFontDatabase::addApplicationFont(PATH+fontName);
    QString family = QFontDatabase::applicationFontFamilies(fontid).at(0);
    font = QFont(family,fontsize);
    //font.setStyleHint(QFont::Helvetica, QFont::PreferAntialias);

    if (off == "")
        offPix = new QImage(PATH+offPic); //preload image
    else
        offPix = new QImage(PATH+off); //preload image

    if (on == "")
        onPix = new QImage(PATH+onPic); //preload image
    else
        onPix = new QImage(PATH+on); //preload image



    badPix = new QImage(PATH+badPic); //preload image

    ratio = (double)(offPix->width())/(offPix->height());
    badGuy = false;

    setWidth(w);


    //this->setStyleSheet("border: 1px solid black");


    QPainter p1(offPix);
    p1.setPen(QPen(Qt::white));
    p1.setFont(font);



    p1.drawText(offPix->rect(), Qt::AlignCenter, text());


    QPainter p2(onPix);
    p2.setPen(QPen(Qt::white));
    p2.setFont(font);
    p2.drawText(onPix->rect(), Qt::AlignCenter, text());

    drawState(false);

}


void button::setWidth(int w)
{


    int h = offPix->height()*w/offPix->width();


    this->resize(w,h);


}


void button::goBadGuy()
{
    QPixmap pix;
    pix = QPixmap::fromImage((*badPix).scaled(width(),height()));
    badGuy = true;
    setPixmap(pix);

}


void button::stopBadGuy()
{
    QPixmap pix;
    pix = QPixmap::fromImage((*offPix).scaled(width(),height()));
    badGuy = false;
    setPixmap(pix);
}


void button::drawState(bool state)
{
    QPixmap pix;

    if(state)
        pix = QPixmap::fromImage((*onPix).scaled(width(),height()));
    else
        pix = QPixmap::fromImage((*offPix).scaled(width(),height()));

    setPixmap(pix);
}


void button::mousePressEvent( QMouseEvent* ev )
{
    QImage image(pixmap()->toImage());
    QColor color(image.pixel(ev->pos()));

    if(badGuy)
    {
        stopBadGuy();
        emit clicked(-1);
        drawState(true);
        return;

    }

    if((color.black()==255)&(color.hslHue()==-1)&(color.hslSaturation()==0)&(ignoreEmptySpace))//transparent area
        return;

    drawState(true);
    emit clicked(id);
}


void button::mouseReleaseEvent(QMouseEvent * ev)
{
    drawState(false);
    emit released(id);
}


button::~button()
{


}

