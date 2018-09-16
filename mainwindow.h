#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login.h"
#include "gamechoice.h"
#include "qstring.h"
#include "game1.h"
#include "game2.h"
#include "countdown.h"
#include "endgame.h"
#include "rules.h"
#include "maintenance.h"
#include "QtNetwork/qnetworkaccessmanager.h"
#include "QtNetwork/QtNetwork"


#define PATH_DEFAULT (QString)"/home/fred/Dropbox/Taf/Cassiopee/cybergame/files/"

#define NETWORK_OK 0
#define NETWORK_FAIL 1
#define NETWORK_NOUSER 2

#define timeToGoHome 10*1000

#define maxNetworkAttempt 5


struct userData_t {
  QString id;
  int level;
  int score1;
  int attempt1;
  int score2;
  int attempt2;
} ;




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int requestData(QString id);
    ~MainWindow();

    userData_t getUserData() const;
    void setAttempt(int gameid,int attempt);
    void newAttempt(int gameid);
    int getAttempt(int gameid);
    void setScore(int gameid,int score);
    int getScore(int gameid);

    QString getId();
    int postData(QString id, int gameId, int score, int attempt);
private:
    Ui::MainWindow *ui;
    login *lg;
    gamechoice *gc;
    game1 *g1;
    game2 *g2;
    countDown *cd;
    endGame *eg;
    maintenance *mt;
    gamerules *gr;

    void getParams(QStringList params);
    QString PATH;
    bool HIDE_CURSOR;


    struct userData_t userData;

    void hideAll();


    void initNetwork();


    QNetworkAccessManager  *manager;

private slots:
    void loginAccepted();
    void gameChosen(int gameId);
    void startGame(int gameId);
    void onGameWon(int gameId, ulong score);
    void onGameLost(int gameId,ulong score);
    void afterEndGame(int gameId);

    void startCountDown(int gameId);
    void goMaintenance();

public slots:
void goHome(QString source);

};

#endif // MAINWINDOW_H
