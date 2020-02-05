#include "MonitorConfig.h"

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

