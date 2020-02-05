#ifndef MONITORCONFIG_H
#define MONITORCONFIG_H

#include <QString>

class MonitorConfig
{
private:
    QString _serverIP;
    int _serverPort;
    int _heartbeat; // 小于0不启用心跳包,否则启用.单位(s)
public:
    MonitorConfig(QString serverIp, int port, int heartbeat);
    ~MonitorConfig();
    QString getServerIP();
    int getServerPort();
    int getHeartbeat();
};

#endif // MONITORCONFIG_H
