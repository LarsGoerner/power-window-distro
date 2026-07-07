#include <QObject>
#include <QString>
#include <QDir>
#include <QFile>
#include "Backlight.hpp"

Backlight::Backlight(QObject * parent) : QObject(parent), mPath("/sys/class/backlight/waveshare_bl/")
{
        QFile maxFile(mPath + "/max_brightness");
        if (maxFile.open(QIODevice::ReadOnly)) {
                mMaxBrightness = maxFile.readAll().trimmed().toInt();
        }
}

int Backlight::brightness() const
{
        if (mPath.isEmpty() || mMaxBrightness <= 0) { return 0; }
        QFile file(mPath + "/actual_brightness");
        if (!file.open(QIODevice::ReadOnly)) { return 0; }
        return file.readAll().trimmed().toInt() * 100 / mMaxBrightness;
}

void Backlight::setBrightness(int perc)
{
        if (mPath.isEmpty() || mMaxBrightness <= 0) { return; }
        int raw = qBound(0, perc * mMaxBrightness / 100, mMaxBrightness);
        writeSysfs("brightness", raw);
        emit brightnessChanged();
}

int Backlight::readSysfs(const QString &file) const
{
        QFile f(mPath + "/" + file);
        if (!f.open(QIODevice::ReadOnly)) { return 0; }
        return f.readAll().trimmed().toInt();
}

void Backlight::writeSysfs(const QString &file, int value)
{
        QFile f(mPath + "/" + file);
        if (f.open(QIODevice::WriteOnly)) {
                f.write(QByteArray::number(value));
        }
}
