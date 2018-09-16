#ifndef CHOICE_H
#define CHOICE_H


#include "qobject.h"
#include "qpainter.h"
#include "qlabel.h"
#include "qthread.h"
#include "qevent.h"
#include <QMouseEvent>
#include "qstringlist.h"
#include "button.h"




class gamechoice:public QLabel
{
    Q_OBJECT

public:
    gamechoice(QWidget *parent = 0, QString filename="", QString fontname="", QString PATH="");
    ~gamechoice();
    void start();


private:
    QString filename;
    QString fontname;
    QString PATH;
    QPixmap* background;
    QFont font;
    std::vector<button*> matrix;
    void generateMatrix(int matrixX,int matrixY,int buttonWidth);
    button *home;
    QTimer *goHomeTimer;


signals:
    void startGame(int);
    void missionCompleted(QString);

private slots:
    void timeout();
    void starter(int gameid);
    void completeMission();

public slots:
    void stopGoHomeTimer();

};








#endif // SPLASH_H
