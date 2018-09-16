#ifndef MAINTENANCE_H
#define MAINTENANCE_H


#include "qobject.h"
#include "qpainter.h"
#include "qlabel.h"
#include "qthread.h"
#include "qevent.h"
#include <QMouseEvent>
#include "qstringlist.h"
#include "button.h"




class maintenance:public QLabel
{
    Q_OBJECT

public:
    maintenance(QWidget *parent = 0, QString filename ="", QString PATH="");
    ~maintenance();
    void start();


private:
    QString filename;
    QPixmap *background;
    QTimer *networkCheck;
    QString PATH;
signals:
    void networkIsBack(QString);
private slots:
    void checkNetwork();

};








#endif // SPLASH_H
