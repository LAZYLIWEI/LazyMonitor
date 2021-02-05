#include <QApplication>
#include "MonitorUtil.h"
#include "MonitorConfig.h"
#include "MonitorSocket.h"
#include "MonitorCamera.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 自启
    MonitorUtil::startMyself();
    // 能否连上服务器
    QString configFileName = QApplication::applicationDirPath() + "/config.ini";
    MonitorConfig config(configFileName);
    MonitorSocket socket(config);
    socket.connectServer();
    return app.exec();
}

