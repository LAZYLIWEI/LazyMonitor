#include "MonitorCamera.h"

MonitorCamera::MonitorCamera(QObject *parent) : QObject(parent)
{
    _camera = new QCamera(this);
    _capture=new QCameraImageCapture(_camera);

    _camera->setCaptureMode(QCamera::CaptureStillImage);
    _capture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    connect(_capture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(slotCameraImageCaptured(int,QImage)));
}

MonitorCamera::~MonitorCamera()
{

}


void MonitorCamera::capture()
{
    if (_camera->status()
            != QCamera::Status::StartingStatus)
    {
        _camera->start();
        _capture->capture();
    }
}


void MonitorCamera::stop()
{
    if (_camera->status()
            != QCamera::Status::StoppingStatus)
    {
        _capture->cancelCapture();
        _camera->stop();
    }
}

void MonitorCamera::slotCameraImageCaptured(int id, QImage image)
{
    QByteArray bytearray;
    QBuffer buffer(&bytearray);
    buffer.open(QIODevice::ReadWrite);
    image.save(&buffer, "JPG");
    //image.save("d:\\test126547.jpg");
    emit imageCaptured(bytearray);
    //this->stop();
}



