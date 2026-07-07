#ifndef DHT_SENSOR_HPP_
#define DHT_SENSOR_HPP_

#include <QObject>
#include <QTimer>

class DhtSensor : public QObject
{
        Q_OBJECT
        Q_PROPERTY(double temperature READ temperature NOTIFY dataChanged)
        Q_PROPERTY(double humidity READ humidity NOTIFY dataChanged)

public:
        explicit DhtSensor(QObject * parent = nullptr);
        double temperature() const;
        double humidity() const;

signals:
        void dataChanged();

private:
        void poll();
        void detectDevice();

        QTimer * mTimer;
        double mTemperature = 0;
        double mHumidity = 0;
        QString mIioPath;
};

#endif // DHT_SENSOR_HPP_