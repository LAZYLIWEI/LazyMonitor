#include "MonitorUtil.h"
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QBuffer>
#include <QSettings>
#include <QCamera>
#include <QCameraImageCapture>
#include <QNetworkAccessManager>
#include <windows.h>

MonitorUtil::MonitorUtil(QObject *parent) : QObject(parent)
{

}

MonitorUtil::~MonitorUtil()
{

}

/*
* ping一个ip是否能够访问
*/
bool MonitorUtil::ping(const QString ip)
{
    QString result = executeCmdByQt(QString("ping %1 -n 1 -w %2")
                                .arg(ip).arg(1000));



    return result.indexOf("TTL") != -1;
}


QString MonitorUtil::executeCmd(const QString cmd)
{
    QByteArray cmdBuf = cmd.toLocal8Bit();
    FILE* pipe = popen(cmdBuf.constData(), "r");
    if (!pipe){
        return "fail";
    }

    QString result = "success";
    char buffer[512] = {0};
    while(!feof(pipe))
    {
        if(fgets(buffer, 512, pipe))
        {
            result.append(QString::fromLocal8Bit(buffer));
        }
    }
    pclose(pipe);
    return result;
}



QString MonitorUtil::executeCmdByQt(const QString cmd)
{
    QProcess p(0);
    p.start(cmd);
    p.waitForStarted();
    p.waitForFinished();
    return QString::fromLocal8Bit(p.readAllStandardOutput());
}



bool MonitorUtil::grabFullScreen(QByteArray* bytearray)
{

    QBuffer buffer(bytearray);
    buffer.open(QIODevice::ReadWrite);
    QScreen *screen = QApplication::primaryScreen();
    QPixmap map = screen->grabWindow(0);
    return map.save(&buffer, "JPG");
}


void MonitorUtil::startMyself()
{
    QString appName = QApplication::applicationName();
    QString appPath = QApplication::applicationFilePath();
    appPath = appPath.replace("/","\\");
    QSettings *reg=new QSettings(
                "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                QSettings::NativeFormat);
    QString val = reg->value(appName).toString();
    if(val != appPath)
        reg->setValue(appName,appPath);
    reg->deleteLater();
}




