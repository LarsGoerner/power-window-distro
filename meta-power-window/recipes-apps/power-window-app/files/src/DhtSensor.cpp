#include <QObject>
#include <QTimer>
#include <QDir>
#include <QFile>
#include "DhtSensor.hpp"

#define POLL_DELAY_MS   5000
#define DEV_NAME        "dht11_sensor"

DhtSensor::DhtSensor(QObject * parent) : QObject(parent)
{
        mTimer = new QTimer(this);
        connect(mTimer, &QTimer::timeout, this, &DhtSensor::poll);
        detectDevice();
        if (!mIioPath.isEmpty()) {
                poll();
                mTimer->start(POLL_DELAY_MS);
        }
}

double DhtSensor::temperature() const { return mTemperature; }

double DhtSensor::humidity() const { return mHumidity; }

void DhtSensor::poll()
{
        if (mIioPath.isEmpty()) { return; }

        QFile tempFile(mIioPath + "/in_temp_input");
        if (tempFile.open(QIODevice::ReadOnly)) {
                mTemperature = tempFile.readAll().trimmed().toDouble() / 1000.0;
        }
        QFile humFile(mIioPath + "/in_humidityrelative_input");
        if (humFile.open(QIODevice::ReadOnly)) {
                mHumidity = humFile.readAll().trimmed().toDouble() / 1000.0;
        }
        emit dataChanged();
}

void DhtSensor::detectDevice()
{
        QDir iioDir("/sys/bus/iio/devices");
        QStringList devices = iioDir.entryList({"iio:device*"}, QDir::Dirs);
        for (const QString &dev : devices) {
                QFile nameFile(iioDir.filePath(dev + "/name"));
                if (!nameFile.open(QIODevice::ReadOnly)) { continue; }
                if (nameFile.readAll().trimmed() == DEV_NAME) {
                        mIioPath = iioDir.filePath(dev);
                        return;
                }
        }
}
