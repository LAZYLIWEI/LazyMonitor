#ifndef MONITORSOCKET_H
#define MONITORSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "MonitorConfig.h"
#include "MonitorCommand.h"
#include "MonitorCamera.h"


class MonitorSocket : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* _socket;
    MonitorConfig& _config;
    QByteArray _recvBuf;
    QTimer* _heartbeatTimer;
    MonitorCamera* _camera;
    bool _disconnect;
private:
    void analyzeData(const QByteArray& buf);
    int byteArrayToInt(const QByteArray &bytes);
    bool enableHeartbeat();

public:
    MonitorSocket(MonitorConfig& config, QObject *parent = 0);
    ~MonitorSocket();
    void connectServer();
    int write(QByteArray byteBuf);


    //////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////// 写消息 ////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////

    /*
    * 发送心跳包
    */
    void sendHeartbeat();
    void sendExecuteCMDResult(const QString result);
    void sendGetFullScreenCMDResult(const QByteArray& result);

signals:

public slots:
    void slotReadyRead();
    void slotDisconnected();
    void slotConnected();
    void slotSendHeartbeat();
    void slotImageCaptured(QByteArray& imageBuf);
};

#endif // MONITORSOCKET_H
