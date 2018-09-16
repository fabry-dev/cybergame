#ifndef BUTTON_H
#define BUTTON_H


#include "qobject.h"
#include "qpainter.h"
#include "qlabel.h"
#include "qthread.h"
#include "qevent.h"
#include <QMouseEvent>
#include "qstringlist.h"
#include "qpushbutton.h"





class button:public QLabel
{
    Q_OBJECT

public:
    button(QWidget* parent = 0, QString txt="OK", int id=-1, int w=10, int fontsize=15, QString on="", QString off="", bool ignoreEmptySpace=true, QString PATH="");
    ~button();
    void setWidth(int w);
    double ratio;
    void goBadGuy();
    void stopBadGuy();

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * ev);

private:
    int id;//button identifier
    void drawState(bool activated);
    QImage *onPix,*offPix,*badPix;
    QFont font;
    bool badGuy;
    bool ignoreEmptySpace;

signals:
    void clicked(int);
    void released(int);

};








#endif
