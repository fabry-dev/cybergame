#ifndef rules_H
#define rules_H


#include "qobject.h"
#include "qpainter.h"
#include "qlabel.h"
#include "qthread.h"
#include "qevent.h"
#include <QMouseEvent>
#include "qstringlist.h"
#include "button.h"




class gamerules:public QLabel
{
    Q_OBJECT

public:
    gamerules(QWidget *parent = 0, QString PATH="");
    ~gamerules();
    void start(int gameId);

protected:
    void mousePressEvent( QMouseEvent* ev );
private:

    QPixmap* background1,*background2;
    int gameId;
    QTimer *goHomeTimer;
    button *arrow;
    QString PATH;


signals:
    void startGame(int);
    void goBackToChoice();


private slots:
    void timeout();
    void done();


public slots:
        void stopGoHomeTimer();
};








#endif // SPLASH_H
