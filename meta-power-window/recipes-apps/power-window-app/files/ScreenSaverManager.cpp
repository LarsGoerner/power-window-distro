#include <QObject>
#include <QTimer>
#include <QEvent>
#include <QFile>
#include <QApplication>
#include <QDebug>
#include "ScreenSaverManager.hpp"

ScreenSaverManager::ScreenSaverManager(int idleTimeoutSecs, QObject * parent) : QObject(parent), mTimeout(idleTimeoutSecs)
{
        qApp->installEventFilter(this);

        mIdleTimer = new QTimer(this);
        mIdleTimer->setSingleShot(true);
        connect(mIdleTimer, &QTimer::timeout, this, [this]() { blankDisplay(true); });
        mIdleTimer->start(mTimeout * 1000);
}

bool ScreenSaverManager::eventFilter(QObject * obj, QEvent * event)
{
        if (event->type() == QEvent::MouseButtonPress) {
                if (mDisplayBlanked) {
                        blankDisplay(false);
                        return true;
                }
                mIdleTimer->start(mTimeout * 1000);
        }
        return QObject::eventFilter(obj, event);
}

void ScreenSaverManager::blankDisplay(bool blank)
{
        if (blank == mDisplayBlanked) { return; }
        mDisplayBlanked = blank;

        // set display blanking
        QFile fbBlank("sys/class/graphics/fb0/blank");
        if (fbBlank.open(QIODevice::WriteOnly)) {
                fbBlank.write(blank ? "1" : "0");
                fbBlank.close();
        }

        // set backlight
        QFile bl("/sys/class/backlight/waveshare_bl/brightness");
        if (bl.open(QIODevice::ReadWrite)) {
                if (blank) {
                        mLastBrightness = bl.readAll().trimmed().toInt();
                        bl.write("0");
                } else {
                        bl.write(QByteArray::number(mLastBrightness));
                }
                bl.close();
        }

        if (!blank) { mIdleTimer->start(mTimeout * 1000); }
}
