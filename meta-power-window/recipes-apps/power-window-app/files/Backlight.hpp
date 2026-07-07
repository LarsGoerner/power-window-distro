#ifndef BACKLIGHT_HPP_
#define BACKLIGHT_HPP_

#include <QObject>
#include <QString>

class Backlight : public QObject
{
        Q_OBJECT
        Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)

public:
        explicit Backlight(QObject * parent = nullptr);
        int brightness() const;
        Q_INVOKABLE void setBrightness(int perc);

signals:
        void brightnessChanged();

private:
        QString mPath;
        int mMaxBrightness = 0;

        int readSysfs(const QString &file) const;
        void writeSysfs(const QString &file, int value);
};

#endif // BACKLIGHT_HPP_