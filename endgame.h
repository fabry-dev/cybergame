#ifndef endGame_H
#define endGame_H


#include "qobject.h"
#include "qpainter.h"
#include "qlabel.h"
#include "qthread.h"
#include "qevent.h"
#include <QMouseEvent>
#include "qstringlist.h"
#include "button.h"





class endGame:public QLabel
{
    Q_OBJECT

public:
    endGame(QWidget *parent = 0, QString filenameWin="", QString fontname="", QString PATH="");
    ~endGame();
    void start(int nugameId,int nuscore);
    void stopGoHomeTimer();

private:
    QString filename;

    QString fontname;
    QPixmap *background;
    QFont font;
    button *home;
    QString PATH;
    int gameId;
    int score;
    QTimer *goHomeTimer;
    int connectionAttempts;
    std::vector<button*> matrix;
    void generateMatrix(int matrixX, int matrixY,int buttonWidth);

    void doneUpdatingScore();

private slots:
    void done();
    void replay();
    void updateScore();
    void timeout();
    void realDone();
signals:

    void doneShowingScore(int gameId);
    void startGame(int);
    void networkError();




};








#endif // SPLASH_H
