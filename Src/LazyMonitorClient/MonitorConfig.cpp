#include "MonitorConfig.h"
#include <QSettings>

MonitorConfig::MonitorConfig(QString configFileName)
{
    QSettings* setting = new QSettings(configFileName, QSettings::IniFormat);
    this->_serverIP = setting->value("server/ip").toString();
    this->_serverPort = setting->value("server/port").toInt();
    this->_heartbeat = setting->value("server/heartbeat").toInt() * 1000;
}

MonitorConfig::MonitorConfig(QString serverIp, int port, int heartbeat)
{
    this->_serverIP = serverIp;
    this->_serverPort = port;
    this->_heartbeat = heartbeat * 1000;
}


MonitorConfig::~MonitorConfig()
{

}

QString MonitorConfig::getServerIP()
{
    return this->_serverIP;
}

int MonitorConfig::getServerPort()
{
    return this->_serverPort;
}

int MonitorConfig::getHeartbeat()
{
    return this->_heartbeat;
}

