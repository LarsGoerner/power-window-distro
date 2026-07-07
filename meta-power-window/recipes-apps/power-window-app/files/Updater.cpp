#include <QObject>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include "Updater.hpp"

static const QString SCRIPT_FILE = "/tmp/update-power-window.sh";
static const QString SCRIPT_LOG = "/tmp/update-power-window.log";

Updater::Updater(QObject * parent) : QObject(parent), mLogFile(SCRIPT_LOG), mPollTimer(new QTimer(this))
{
        connect(mPollTimer, &QTimer::timeout, this, &Updater::pollLog);
}

QString Updater::updateLog() const { return mUpdateLog; }

bool Updater::updateRunning() const { return mUpdateRunning; }

void Updater::startUpdate()
{
        if (mUpdateRunning) { return; }

        QFile sf(SCRIPT_FILE);
        if (!sf.open(QIODevice::WriteOnly)) { return; }
        sf.write(
                "#!/bin/sh\n"
                "{\n"
                "  echo '=== Update ==='\n"
                "  opkg update 2>&1\n"
                "  echo '=== Upgrade ==='\n"
                "  opkg upgrade 2>&1\n"
                "  echo ''\n"
                "  echo '=== Done ==='\n"
                "} > /tmp/update-power-window.log 2>&1\n"
        );
        sf.close();
        sf.setPermissions(QFileDevice::ExeOwner | QFileDevice::ExeGroup |
                          QFileDevice::ExeOther | QFileDevice::ReadOwner |
                          QFileDevice::WriteOwner);
        QFile::remove(mLogFile);
        mUpdateLog.clear();
        mLastPos = 0;
        mUpdateRunning = true;
        emit updateRunningChanged();
        emit updateLogChanged();

        QProcess::startDetached("/bin/sh", { SCRIPT_FILE });
        mPollTimer->start(500);
}

void Updater::pollLog()
{
        QFile f(mLogFile);
        if (!f.open(QIODevice::ReadOnly)) { return; }

        if (!f.seek(mLastPos)) { return; }
        QByteArray newData = f.readAll();
        mLastPos = f.pos();
        f.close();

        if (newData.isEmpty()) { return; }

        mUpdateLog += QString::fromUtf8(newData);
        emit updateLogChanged();

        if (mUpdateLog.contains("=== Done ===")) {
                mPollTimer->stop();
                mUpdateRunning = false;
                emit updateRunningChanged();
        }
}