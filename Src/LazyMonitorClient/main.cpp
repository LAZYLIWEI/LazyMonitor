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
    MonitorConfig config("47.92.213.250", 30000, 120);
    //MonitorConfig config("127.0.0.1", 30000, 120);
    MonitorSocket socket(config);
    socket.connectServer();
    return app.exec();
}

