#ifndef WIFI_MANAGER_HPP_
#define WIFI_MANAGER_HPP_

#include <QObject>
#include <QStringList>
#include <QProcess>
#include <QTimer>

class WifiManager : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QStringList scanResults READ scanResults NOTIFY scanResultsChanged)
        Q_PROPERTY(bool scanning READ scanning NOTIFY scanningChanged)
        Q_PROPERTY(bool connecting READ connecting NOTIFY connectingChanged)
        Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
        explicit WifiManager(QObject * parent = nullptr);
        QStringList scanResults() const;
        bool scanning() const;
        bool connecting() const;
        QString statusMessage() const;
        Q_INVOKABLE void startScan();
        Q_INVOKABLE void connectToNetwork(const QString &ssid, const QString &password);

signals:
        void scanResultsChanged();
        void scanningChanged();
        void connectingChanged();
        void statusMessageChanged();

private slots:
        void onScanFinished(int exitCode);
        void onScanResultsReady();
        void onConnectFinished(int exitCode);

private:
        QStringList mScanResults;
        bool mScanning = false;
        bool mConnecting = false;
        QString mStatusMessage;
        QString mInterface = "wlan0";
        QProcess * mProcess = nullptr;

        void runWpaCli(const QStringList &args);
        void setStatus(const QString &msg);
        void clearProcess();
};

#endif // WIFI_MANAGER_HPP_