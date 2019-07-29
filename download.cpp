#include "download.h"
#include "ui_download.h"
#include <qdebug.h>


Download::Download(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Download)
{
    ui->setupUi(this);
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    this->setWindowFlags(Qt::FramelessWindowHint);

    //读取服务器配置信息
    Update.start_update();
    ui->textBrowser->append("welcome!  " + Update.config.Remarks);
    ui->textBrowser->append("The Last_version:   "+Update.config.Last_version);
    ui->textBrowser->append("Changelog:   " + Update.config.Changelog);

    m_manager = new QNetworkAccessManager();
    m_downloadDir = QDir::current();
    m_fileName = "";
    m_userAgentString = QString ("%1/%2 (Qt; zjzl)").arg (qApp->applicationName(),qApp->applicationVersion());

}

Download::~Download()
{
    delete ui;
}

void Download::startDownload(const QUrl &url)
{
    /* Reset UI */
    ui->progressBar->setValue (0);
    ui->downloadLabel->setText (tr ("Downloading updates"));
    ui->timeLabel->setText (tr ("Time remaining") + ": " + tr ("unknown"));

    /* Configure the network request */
    QNetworkRequest request (url);
    if (!m_userAgentString.isEmpty())
        request.setRawHeader ("User-Agent", m_userAgentString.toUtf8());

    /* Start download */
    m_reply = m_manager->get (request);
    m_startTime = QDateTime::currentDateTime().toTime_t();

    /* Ensure that downloads directory exists */
    if (!m_downloadDir.exists())
        m_downloadDir.mkpath (".");

    /* Remove old downloads */
    QFile::remove (m_downloadDir.filePath (m_fileName));
    QFile::remove (m_downloadDir.filePath (m_fileName + PARTIAL_DOWN));

    /* Update UI when download progress changes or download finishes */
    connect (m_reply, SIGNAL (downloadProgress (qint64, qint64)),
             this,      SLOT (updateProgress   (qint64, qint64)));
    connect (m_reply, SIGNAL (finished ()),
             this,      SLOT (finished ()));
    connect (m_reply, SIGNAL (redirected       (QUrl)),
             this,      SLOT (startDownload    (QUrl)));

    showNormal();
}

void Download::finished()
{
    /* Rename file */
    QFile::rename (m_downloadDir.filePath (m_fileName + PARTIAL_DOWN),
                   m_downloadDir.filePath (m_fileName));
    /* Install the update */
    m_reply->close();
    ui->textBrowser->append("download successful");
    is_downloadok = true;
}



void Download::setFileName(const QString &file)
{
    m_fileName = file;

    if (m_fileName.isEmpty())
        m_fileName = "update.bin";
}

void Download::calculateSizes(qint64 received, qint64 total)
{
    QString totalSize;
    QString receivedSize;

    if (total < 1024)
        totalSize = tr ("%1 bytes").arg (total);

    else if (total < 1048576)
        totalSize = tr ("%1 KB").arg (round (total / 1024));

    else
        totalSize = tr ("%1 MB").arg (round (total / 1048576));

    if (received < 1024)
        receivedSize = tr ("%1 bytes").arg (received);

    else if (received < 1048576)
        receivedSize = tr ("%1 KB").arg (received / 1024);

    else
        receivedSize = tr ("%1 MB").arg (received / 1048576);

     ui->downloadLabel->setText (tr ("Downloading updates")
                                  + " (" + receivedSize + " " + tr ("of")
                                 + " " + totalSize + ")");
}

void Download::updateProgress(qint64 received, qint64 total)
{
    if (total > 0) {
        ui->progressBar->setMinimum (0);
        ui->progressBar->setMaximum (100);
        ui->progressBar->setValue ((received * 100) / total);

        calculateSizes (received, total);
        calculateTimeRemaining (received, total);
        saveFile (received, total);
    }

    else {
        ui->progressBar->setMinimum (0);
        ui->progressBar->setMaximum (0);
        ui->progressBar->setValue (-1);
        ui->downloadLabel->setText (tr ("Downloading Updates") + "...");
        ui->timeLabel->setText (QString ("%1: %2")
                                  .arg (tr ("Time Remaining"))
                                  .arg (tr ("Unknown")));
    }
}

void Download::calculateTimeRemaining(qint64 received, qint64 total)
{
    uint difference = QDateTime::currentDateTime().toTime_t() - m_startTime;

    if (difference > 0) {
        QString timeString;
        qreal timeRemaining = total / (received / difference);

        if (timeRemaining > 7200) {
            timeRemaining /= 3600;
            int hours = int (timeRemaining + 0.5);

            if (hours > 1)
                timeString = tr ("about %1 hours").arg (hours);
            else
                timeString = tr ("about one hour");
        }

        else if (timeRemaining > 60) {
            timeRemaining /= 60;
            int minutes = int (timeRemaining + 0.5);

            if (minutes > 1)
                timeString = tr ("%1 minutes").arg (minutes);
            else
                timeString = tr ("1 minute");
        }

        else if (timeRemaining <= 60) {
            int seconds = int (timeRemaining + 0.5);

            if (seconds > 1)
                timeString = tr ("%1 seconds").arg (seconds);
            else
                timeString = tr ("1 second");
        }

        ui->timeLabel->setText (tr ("Time remaining") + ": " + timeString);
    }
}

void Download::saveFile(qint64 received, qint64 total)
{
    Q_UNUSED (received);
    Q_UNUSED (total);

    /* Check if we need to redirect */
    QUrl url = m_reply->attribute (
                   QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (!url.isEmpty()) {
        startDownload (url);
        return;
    }

    /* Save downloaded data to disk */
    QFile file (m_downloadDir.filePath (m_fileName + PARTIAL_DOWN));
    if (file.open (QIODevice::WriteOnly | QIODevice::Append)) {
        file.write (m_reply->readAll());
        file.flush();
        file.close();
    }
}


void Download::on_stopButton_released()
{
    ui->textBrowser->append(tr("There are %1 files in this update.").arg(Update.config.Download_number));
    Update.check_url();


    for (int i = 0; i < Update.config.Update_URL.size(); i ++) {
        is_downloadok = false;
        QUrl down_url(Update.config.Update_URL.at(i));
        setFileName(down_url.toString().split ("/").last());
        startDownload(down_url);
        while(!is_downloadok)
        {
         QApplication::processEvents();//异步等待
        }
    }


}

void Download::on_cancelButton_released()
{
    qApp->exit();
}














void Download::mouseMoveEvent(QMouseEvent *e)
{
    this->move(e->globalPos() - m_pointStart);
}

void Download::mousePressEvent(QMouseEvent *e)
{
    m_pointPress = e->globalPos();
    m_pointStart = m_pointPress - this->pos();
}

