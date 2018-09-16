#ifndef game2_H
#define game2_H


#include "qobject.h"
#include "qpainter.h"
#include "qlabel.h"
#include "qthread.h"
#include "qevent.h"
#include <QMouseEvent>
#include "qstringlist.h"
#include "button.h"
#include "qtimer.h"



class game2:public QLabel
{
    Q_OBJECT

public:
    game2(QWidget *parent = 0, QString filename="", QString fontname="", QString PATH="");
    ~game2();
    void start(int level);


private:
    QString filename;
    QString fontname;
    QString PATH;
    QPixmap* background;
    QFont font;
    std::vector<button*> matrix;
    void generateMatrix(int matrixX,int matrixY,int buttonWidth);

    QLabel *lblscore,*lbltime;
    void updScore(int nu);
    void updTime(ulong nu);

    QTimer *clock;
    ulong clockCount;
    int score;
    bool badGuy,gameActive;
    int badPos;
    void pauseTimer(void);
    void unpauseTimer(void);
    int level;
    void loadParameters(void);
    std::vector<int>badGuyTimeOn;


private slots:
    void tick();
    void onClick(int id);
    void nextStep(void);
    void stop();

signals:
    void won(int game,ulong score);
    void lost(int game,ulong score);

};








#endif // SPLASH_H
