#ifndef MEMORY_H
#define MEMORY_H


#include "qobject.h"
#include "qpainter.h"
#include "qlabel.h"
#include "qthread.h"
#include "qevent.h"
#include <QMouseEvent>
#include "qstringlist.h"
#include "qpushbutton.h"





class memoryTile:public QLabel
{
    Q_OBJECT

public:
    memoryTile(QWidget* parent = 0, int type=0, int id=-1, int w=10, int fontsize=15, QString PATH="");
    ~memoryTile();
    void setWidth(int w);
    double ratio;
    void setSymbol(int symbol);
    void goHidden();
    void goWon();
    bool isSelected();
    int getType();

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * ev);

private:
    int state;//0-4 state
    int type;//which symbol
    int id;//memoryTile identifier
    int w;//desired width
    QString PATH;
    void drawState();

    void changeState();

    QImage *onPix,*offPix,*hiddenPix;



    QFont font;




signals:
    void clicked(int);
    void released(int);

};








#endif
