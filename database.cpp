#include "database.h"
#include <QLatin1String>
#include <QString>
#include <QDebug>
DataBase::DataBase(QObject *parent) : QObject(parent)
{
    qDebug()<<"init date base!";
}
DataBase::~DataBase(){
    db.close();
}

bool DataBase::open()
{
    db.setHostName("nas.indooruwb.xyz");
    db.setPort(3307);
    db.setUserName("root");
    db.setPassword("zjzl123456");
    db.setDatabaseName("zlkj");
    bool ok = db.open();
    return ok;
}
void DataBase::close()
{
    db.close();
}
QSqlError DataBase::Cmd(QString cmd){
    if (!q.exec(cmd))
    {
            qDebug()<< q.lastError();
            return q.lastError();
    }
}
