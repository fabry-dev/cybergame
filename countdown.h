#ifndef COUNTDOWN_H
#define COUNTDOWN_H


#include "qobject.h"
#include "qpainter.h"
#include "qlabel.h"
#include "qthread.h"
#include "qevent.h"
#include <QMouseEvent>
#include "qstringlist.h"
#include "button.h"




class countDown:public QLabel
{
    Q_OBJECT

public:
    countDown(QWidget *parent = 0,QString filename="",QString fontname="",QString PATH="");
    ~countDown();
    void start(int gameId);


private:
    QString filename;
    QString fontname;
    QString PATH;
    QPixmap* background;
    QFont font;
    QLabel *frame;
    QImage *framePix1,*framePix2,*framePix3;
    int count;
    int game;

signals:
    void done(int);


private slots:
    void showNextDigit();




};








#endif // SPLASH_H
