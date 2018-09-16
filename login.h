#ifndef LOGIN_H
#define LOGIN_H


#include "qobject.h"
#include "qpainter.h"
#include "qlabel.h"
#include "qthread.h"
#include "qevent.h"
#include <QMouseEvent>
#include "qstringlist.h"
#include "button.h"




class login:public QLabel
{
    Q_OBJECT

public:
    login(QWidget *parent = 0, QString filename="", QString fontname="", QString PATH="");
    ~login();
    void start();


private:
    QString filename;
    QString fontname;
    QString PATH;
    QPixmap* background;
    QFont font;
    std::vector<button*> matrix;
    void generateMatrix(int matrixX,int matrixY,int buttonWidth);
    void updLogin(int nu);
    QString loginValue;
    QLabel *txt;

    bool lock;
    int connectionAttempts;
protected:
    void mousePressEvent(QMouseEvent* ev);

signals:
    void sendLogin();
    void networkError();

private slots:
    void onClick(int);
    void preSend();
    void validateLogin();



};








#endif // SPLASH_H
