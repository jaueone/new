#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>
#include <database.h>
class update_config
{
public:
    QString MAC_ADDR;
    int IS_LEGAL;
    QString Last_version;
    int Download_number;
    QString Changelog;
    int Mandatory;
    QString Create_time;
    QString Remarks;
    QList<QString> Update_URL;
};
class update : public QObject
{
    Q_OBJECT
public:
    explicit update(QObject *parent = nullptr);
    QString getMacAdress();
    void start_update();
    void check_url();
    DataBase *DB;
    update_config config;
private:
    QString MAC_Addr;
signals:

public slots:
};

#endif // UPDATE_H
