#ifndef UPDATER_HPP_
#define UPDATER_HPP_

#include <QObject>
#include <QString>
#include <QTimer>

class Updater : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QString updateLog READ updateLog NOTIFY updateLogChanged)
        Q_PROPERTY(bool updateRunning READ updateRunning NOTIFY updateRunningChanged)

public:
        explicit Updater(QObject * parent = nullptr);
        QString updateLog() const;
        bool updateRunning() const;
        Q_INVOKABLE void startUpdate();

signals:
        void updateLogChanged();
        void updateRunningChanged();

private slots:
        void pollLog();

private:
        QString mLogFile;
        QString mUpdateLog;
        bool mUpdateRunning = false;
        QTimer * mPollTimer;
        qint64 mLastPos = 0;
};

#endif // UPDATER_HPP_