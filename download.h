#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QWidget>

#include <QtNetwork>
#include <QMessageBox>
#include <QMouseEvent>
#include <QAction>
#include "update.h"
#include "database.h"
namespace Ui {
class Download;
}

static const QString PARTIAL_DOWN (".part");

class Download : public QWidget
{
    Q_OBJECT

public:
    explicit Download(QWidget *parent = nullptr);
    ~Download();
signals:
    void downloadFinished();
private slots:
    void startDownload (const QUrl& url);
    void finished();
    void setFileName (const QString& file);
    void calculateSizes (qint64 received, qint64 total);
    void updateProgress (qint64 received, qint64 total);
    void calculateTimeRemaining (qint64 received, qint64 total);
    void saveFile (qint64 received, qint64 total);


    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void on_stopButton_released();

    void on_cancelButton_released();

private:
    Ui::Download *ui;
    QPoint m_pointStart;
    QPoint m_pointPress;
    class update Update;

    QString m_url;
    uint m_startTime;
    QDir m_downloadDir;
    QString m_fileName;

    QNetworkReply* m_reply;
    QString m_userAgentString;

    bool m_useCustomProcedures;
    bool m_mandatoryUpdate;

    QNetworkAccessManager* m_manager;

    bool is_downloadok;

};

#endif // DOWNLOAD_H
