#ifndef MONITORCAMERA_H
#define MONITORCAMERA_H

#include <QObject>
#include <QCamera>
#include <QCameraImageCapture>
#include <QDebug>
#include <QImage>
#include <QBuffer>


class MonitorCamera : public QObject
{
    Q_OBJECT
private:
    QCamera* _camera;
    QCameraImageCapture* _capture;
public:
    explicit MonitorCamera(QObject *parent = 0);
    ~MonitorCamera();

    void capture();
    void stop();

signals:
    void imageCaptured(QByteArray& imageBuf);
public slots:
    void slotCameraImageCaptured(int id, QImage image);
};

#endif // MONITORCAMERA_H
