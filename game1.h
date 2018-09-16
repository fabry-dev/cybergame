#ifndef GAME1_H
#define GAME1_H


#include "qobject.h"
#include "qpainter.h"
#include "qlabel.h"
#include "qthread.h"
#include "qevent.h"
#include <QMouseEvent>
#include "qstringlist.h"
#include "memorytile.h"
#include "qtimer.h"



class game1:public QLabel
{
    Q_OBJECT

public:
    game1(QWidget *parent = 0, QString filename="", QString fontname="", QString PATH="");
    ~game1();
    void start(int level);


private:
    QString filename;
    QString fontname;
    QString PATH;
    QPixmap* background;
    QFont font;
    std::vector<memoryTile*> matrix;
    void generateMatrix(int matrixX,int matrixY,int buttonWidth);

    QLabel *lblscore,*lbltime;
    void updScore(int nu);
    void updTime(ulong nu);


    ulong clockCount;
    int score;
    QList<int> selectedTiles;
    QList<int> wonTiles;
    void pauseTimer(void);
    void unpauseTimer(void);
    QTimer *clock;





private slots:
    void goFullHidden();
    void tick();
    void onClick(int id);
    void stop();


signals:
    void won(int game,ulong score);
    void lost(int game,ulong score);


};








#endif // SPLASH_H
