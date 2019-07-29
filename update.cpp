#include "update.h"
#include <QNetworkInterface>
#include <QDebug>

update::update(QObject *parent) : QObject(parent)
{
    DB = new DataBase;
    MAC_Addr = getMacAdress();
    qDebug() << DB->open();
}

QString update::getMacAdress()
{
    QList<QNetworkInterface> NetList; //网卡链表
     int NetCount = 0; //网卡个数
     int Neti = 0;
     QNetworkInterface thisNet; //所要使用的网卡
     NetList = QNetworkInterface::allInterfaces();//获取所有网卡信息
     NetCount = NetList.count(); //统计网卡个数
     for(Neti = 0;Neti < NetCount; Neti++){ //遍历所有网卡
          if(NetList[Neti].isValid()){ //判断该网卡是否是合法
              thisNet = NetList[Neti]; //将该网卡置为当前网卡
              break;
          }
     }
     return ( thisNet.hardwareAddress() ); //获取该网卡的MAC
}

void update::start_update()
{
    QString info =QString("SELECT * FROM device_table WHERE mac_addr = \'70:85:C2:4B:A5:07\'");
    DB->q.exec(info);
    qDebug() << DB->q.next();

    config.MAC_ADDR =  DB->q.value(1).toString();
    config.IS_LEGAL =  DB->q.value(2).toInt();
    config.Last_version = DB->q.value(3).toString();
    config.Download_number = DB->q.value(4).toInt();
    config.Changelog =  DB->q.value(5).toString();
    config.Mandatory =  DB->q.value(6).toInt();
    config.Create_time = DB->q.value(7).toString();
    config.Remarks= DB->q.value(8).toString();
    //qDebug() << config.MAC_ADDR << config.IS_LEGAL << config.Last_version << config.Download_number << config.Changelog;
    //qDebug() << config.Mandatory << config.Create_time << config.Remarks;
}

void update::check_url()
{
    QString info =QString("SELECT URL FROM Download_Url");
    DB->q.exec(info);
    for (int i=0;i < DB->q.size();i++) {
        qDebug() << DB->q.next();
        config.Update_URL.append(DB->q.value(0).toString());
        qDebug() << DB->q.value(0).toString();
    }

}
