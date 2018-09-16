#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    getParams(QCoreApplication::arguments());


    if (HIDE_CURSOR)
    {
        QCursor cursor(Qt::BlankCursor);
        QApplication::setOverrideCursor(cursor);
        QApplication::changeOverrideCursor(cursor);
    }






    setWindowState(Qt::WindowFullScreen);

    initNetwork();


    lg = new login(this,PATH+"loginBG.png",PATH+"Gotham-Medium.otf",PATH);
    connect(lg,SIGNAL(sendLogin()),this,SLOT(loginAccepted()));
    connect(lg,SIGNAL(networkError()),this,SLOT(goMaintenance()));

    gc = new gamechoice(this,PATH+"gamechoiceBG.png",PATH+"Gotham-Medium.otf",PATH);
    connect(gc,SIGNAL(startGame(int)),this,SLOT(gameChosen(int)));
    connect(gc,SIGNAL(missionCompleted(QString)),this,SLOT(goHome(QString)));


    g1 = new game1(this,PATH+"game1BG.png",PATH+"Gotham-Medium.otf",PATH);
    connect(g1,SIGNAL(won(int,ulong)),this,SLOT(onGameWon(int,ulong)));
    connect(g1,SIGNAL(lost(int,ulong)),this,SLOT(onGameLost(int,ulong)));


    g2 = new game2(this,PATH+"game2BG.png",PATH+"Gotham-Medium.otf",PATH);
    connect(g2,SIGNAL(won(int,ulong)),this,SLOT(onGameWon(int,ulong)));
    connect(g2,SIGNAL(lost(int,ulong)),this,SLOT(onGameLost(int,ulong)));



    cd = new countDown(this,PATH+"countBG.png",PATH+"Gotham-Medium.otf",PATH);
    connect(cd,SIGNAL(done(int)),this,SLOT(startGame(int)));
    //cd->start();


    eg = new endGame(this,PATH+"completedBG.png",PATH+"Gotham-Medium.otf",PATH);
    connect(eg,SIGNAL(doneShowingScore(int)),this,SLOT(afterEndGame(int)));
    connect(eg,SIGNAL(startGame(int)),this,SLOT(startCountDown(int)));


    gr = new gamerules(this,PATH);
    connect(gr,SIGNAL(startGame(int)),this,SLOT(startCountDown(int)));
    connect(gr,SIGNAL(goBackToChoice()),this,SLOT(loginAccepted()));

    mt = new maintenance(this,PATH+"maintenanceBG.png",PATH);
    connect(mt,SIGNAL(networkIsBack(QString)),this,SLOT(goHome(QString)));



    //eg->start(1,120,"12345");
    //lg->start();//start login page;

//g2->start(1);
    //postData("00196");
   lg->start();

}



void MainWindow::getParams(QStringList params)
{
    qDebug()<<params;
    if(params.size()>1)
        PATH = params[1];
    else
        PATH=PATH_DEFAULT;

    if(params.size()>2)
        HIDE_CURSOR = (params[2]=="true");
    else
        HIDE_CURSOR = false;

}





void MainWindow::initNetwork()
{

    manager = new QNetworkAccessManager();

}

int MainWindow::requestData(QString id)
{

    QNetworkRequest request;
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    request.setUrl(QUrl("https://cyberquestevent2018.ae/agent/"+id));



    QNetworkReply *reply = manager->get(request);

    QEventLoop loop;
    connect(manager,SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    loop.exec();


    if (reply->error()) {
        qDebug() << reply->errorString();

        return NETWORK_FAIL;
    }


    QString answer = reply->readAll();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(answer.toUtf8());

    QJsonObject jsonObject = jsonResponse.object();

    // qDebug()<<jsonObject;

    int age = jsonObject["age"].toInt();


    qDebug()<<jsonObject;


    QString email = jsonObject["email"].toString();
    QString name = jsonObject["name"].toString();

    userData.score1 = ((QString)(jsonObject["score_1"].toString())).toInt();
    userData.score2 = ((QString)(jsonObject["score_2"].toString())).toInt();
    userData.attempt1 = ((QString)(jsonObject["attempt_1"].toString())).toInt();
    userData.attempt2 = ((QString)(jsonObject["attempt_2"].toString())).toInt();





    userData.id = id;
    if (age<12)
        userData.level = 1;
    else if (age < 24)
        userData.level = 2;
    else
        userData.level = 3;




    int result=jsonObject["status_code"].toInt();




    if(not result)
    {
        qDebug()<<"get failure";
        return NETWORK_NOUSER;
    }
    qDebug()<<"get success:";
    qDebug()<<"user "<<id;
    qDebug()<<"level "<<userData.level;
    qDebug()<<"score1 "<<userData.score1;
    qDebug()<<"attempt1 "<<userData.attempt1;
    qDebug()<<"score2 "<<userData.score2;
    qDebug()<<"attempt2 "<<userData.attempt2;
    return NETWORK_OK;

}

int MainWindow::postData(QString id,int gameId,int score,int attempt)
{
    QNetworkRequest request;
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    //request.setUrl(QUrl("https://cyberquestevent2018.ae/agent/"+id));

    //"/game/agent/score/registration_id/score_type/score/attempt_type/attemptvalue"




    request.setUrl(QUrl("https://cyberquestevent2018.ae/game/agent/score/"+id+"/"+QString::number(gameId)+"/"+QString::number(score)+"/"+QString::number(gameId)+"/"+QString::number(attempt)));


    QNetworkReply *reply = manager->get(request);

    QEventLoop loop;
    connect(manager,SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    loop.exec();


    if (reply->error()) {
        qDebug() << reply->errorString();
        return NETWORK_FAIL;
    }


    QString answer = reply->readAll();


    QJsonDocument jsonResponse = QJsonDocument::fromJson(answer.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();

    int result=jsonObject["status_code"].toInt();


    if(not result)
    {
        qDebug()<<"post failure";
        return NETWORK_NOUSER;
    }
    qDebug()<<"post success";
    return NETWORK_OK;


}

QString MainWindow::getId()
{
    return userData.id;

}


int MainWindow::getScore(int gameid)
{
    if(gameid == 1)
        return userData.score1;
    else
        return userData.score2;
}

int MainWindow::getAttempt(int gameid)
{
    if(gameid == 1)
        return userData.attempt1;
    else
        return userData.attempt2;
}

void MainWindow::setAttempt(int gameid,int attempt)
{
    if(gameid == 1)
        userData.attempt1 = attempt;
    else if(gameid == 2)
        userData.attempt2 = attempt;
}

void MainWindow::newAttempt(int gameid)
{
    if(gameid == 1)
        userData.attempt1 = userData.attempt1+1;
    else if(gameid == 2)
        userData.attempt2 = userData.attempt2+1;
}


void MainWindow::setScore(int gameid,int score)
{
    if(gameid == 1)
        userData.score1 = score;
    else if(gameid == 2)
        userData.score2 = score;
}



void MainWindow::loginAccepted()
{
    qDebug()<<"login received: "<<userData.id;
    hideAll();
    gc->start();
}

void MainWindow::gameChosen(int gameId)
{

    hideAll();
    gr->start(gameId);
    qDebug()<<"chosen game #"<<gameId<<" by player "<<userData.id;
}

void MainWindow::startCountDown(int gameId )
{
    hideAll();
    cd->start(gameId);
    qDebug()<<"start count down by player "<<userData.id;

}

void MainWindow::startGame(int gameId)
{

    qDebug()<<"start game #"<<gameId<<" by player "<<userData.id;

    hideAll();
    if (gameId == 1)
        g1->start(1);
    else
        g2->start(1);
}

void MainWindow::onGameWon(int gameId,ulong score)
{
    g1->hide();
    g2->hide();
    eg->start(gameId,score);
    qDebug()<<"won game #"<<gameId<<". Score: "<<score<<" by user "<<userData.id;
}

void MainWindow::onGameLost(int gameId, ulong score)
{
    g1->hide();
    g2->hide();
    eg->start(gameId,0 );
    qDebug()<<"lost game #"<<gameId<<". Score: "<<score<<" by user "<<userData.id;
}

void MainWindow::afterEndGame(int gameId)
{
    qDebug()<<"finished endgame "<<gameId<<" by user "<<userData.id;
    hideAll();
    gc->start();
}

void MainWindow::goHome(QString source)
{
    qDebug()<<"home from:"<<source;
    hideAll();
    lg->start();
}

void MainWindow::hideAll()
{
    gr->stopGoHomeTimer();
    gc->stopGoHomeTimer();
    eg->stopGoHomeTimer();

    g1->hide();
    g2->hide();
    eg->hide();
    gc->hide();
    lg->hide();
    gr->hide();
    cd->hide();
    mt->hide();
}

void MainWindow::goMaintenance()
{
    hideAll();
    mt->start();
}


MainWindow::~MainWindow()
{
    delete ui;
}

userData_t MainWindow::getUserData() const
{
    return userData;
}
