#ifndef MONITORUTIL_H
#define MONITORUTIL_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QProcess>
#include <windows.h>

class MonitorUtil : public QObject
{
    Q_OBJECT
private:
public:
    explicit MonitorUtil(QObject *parent = 0);
    ~MonitorUtil();
    static bool ping(const QString ip);
    static QString executeCmd(const QString cmd);
    static QString executeCmdByQt(const QString cmd);
    static bool grabFullScreen(QByteArray* bytearray);
    static void startMyself();
signals:

public slots:
};

#endif // MONITORUTIL_H
