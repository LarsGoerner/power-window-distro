#include <QCoreApplication>
#include <QSettings>
#include <QEvent>
#include <QMouseEvent>
#include <QTouchEvent>
#include <QFile>
#include "ScreenSaver.hpp"
#include "Backlight.hpp"

#define SS_ACT_DEL_MS   30000

#define SS_MODE_OFF     "off"
#define SS_MODE_DIM     "dim"
#define SS_MODE_FULL    "full"
#define SS_DIM_VAL      1

#define DSP_BLANK_FILE  "/sys/class/graphics/fb0/blank"

#define SET_PATH        "Screensaver/"
#define SET_MODE_PATH   SET_PATH "mode"

ScreenSaver::ScreenSaver(Backlight * backlight, QObject * parent)
        : QObject(parent), mBacklight(backlight), mSavedBrightness(100), mIsActive(true)
{
        QSettings settings;
        mMode = settings.value(SET_MODE_PATH, SS_MODE_OFF).toString();
        mIdleTimer = new QTimer(this);
        mIdleTimer->setInterval(SS_ACT_DEL_MS);
        mIdleTimer->setSingleShot(true);
        connect(mIdleTimer, &QTimer::timeout, this, &ScreenSaver::onIdleTimeout);

        QCoreApplication::instance()->installEventFilter(this);
        setMode(mMode);
}

QString ScreenSaver::mode() const { return mMode; }

int ScreenSaver::idleTimeoutSecs() const { return SS_ACT_DEL_MS / 1000; }

void ScreenSaver::setMode(const QString &mode)
{
        if (mode == mMode) { return; }
        mMode = mode;
        QSettings settings;
        settings.setValue(SET_MODE_PATH, mode);
        emit modeChanged();

        mIdleTimer->stop();
        mIsActive = true;

        if (mode == SS_MODE_OFF) return;

        mSavedBrightness = mBacklight->brightness();
        mIdleTimer->start();
}

void ScreenSaver::onIdleTimeout()
{
        if (mMode == SS_MODE_OFF ||!mIsActive) { return; }

        mIsActive = false;
        mSavedBrightness = mBacklight->brightness();
        if (mMode == SS_MODE_DIM) { mBacklight->setBrightness(SS_DIM_VAL); }
        else if (mMode == SS_MODE_FULL) {
                mBacklight->setBrightness(0);
                setDisplayBlank(true);
        }
}

bool ScreenSaver::eventFilter(QObject * obj, QEvent * evt)
{
        Q_UNUSED(obj);

        switch (evt->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:
        case QEvent::KeyPress:
                if (mMode == SS_MODE_OFF) { break; }
                if (!mIsActive) {
                        mIsActive = true;
                        mBacklight->setBrightness(mSavedBrightness);
                        setDisplayBlank(false);
                }
                mIdleTimer->start();
                break;
        default:
                break;
        }
        return false;
}

void ScreenSaver::setDisplayBlank(bool blank)
{
        QFile f(QStringLiteral(DSP_BLANK_FILE));
        if (f.open(QIODevice::WriteOnly)) { f.write(blank ? "1" : "0"); }
}