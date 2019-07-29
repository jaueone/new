#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QDebug>
#include <QString>

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();
    bool open();
    void close();
    QSqlError Cmd(QString cmd);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    QSqlQuery q;
private:
signals:

public slots:
};

#endif // DATABASE_H
