#include "WifiManager.hpp"

WifiManager::WifiManager(QObject * parent) : QObject(parent) {}

QStringList WifiManager::scanResults() const { return mScanResults; }

bool WifiManager::scanning() const { return mScanning; }

bool WifiManager::connecting() const { return mConnecting; }

QString WifiManager::statusMessage() const { return mStatusMessage; }

void WifiManager::startScan()
{
        if (mScanning || mConnecting) { return; }
        mScanResults.clear();
        emit scanResultsChanged();
        mScanning = true;
        emit scanningChanged();
        setStatus("Scanning...");
        runWpaCli({"-i", mInterface, "scan"});
}

void WifiManager::connectToNetwork(const QString &ssid, const QString &password)
{
        if (mScanning || mConnecting) { return; }
        mConnecting = true;
        emit connectingChanged();
        setStatus("Verbinde...");

        QString cmd = QString(
                "HASH=$(wpa_passphrase \"%1\" \"%2\" | grep -o 'psk=[0-9a-f]\\{64\\}' | cut -d= -f2) &&"
                "wpa_cli -i %3 remove_network 0 2>/dev/null; "
                "NETID=$(wpa_cli -i %3 add_network | tail -1) && "
                "wpa_cli -i %3 set_network $NETID ssid '\"%1\"' && "
                "wpa_cli -i %3 set_network $NETID psk $HASH && "
                "wpa_cli -i %3 enable_network $NETID && "
                "wpa_cli -i %3 save_config"
        ).arg(ssid, password, mInterface);
        mProcess = new QProcess(this);
        connect(mProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, &WifiManager::onConnectFinished);
        mProcess->start("/bin/sh", {"-c", cmd});
}

void WifiManager::onScanFinished(int exitCode)
{
        if (exitCode != 0) {
                mScanning = false;
                emit scanningChanged();
                setStatus("Scan fehlgeschlagen");
                clearProcess();
                return;
        }
        setStatus("Suche nach Netzwerken...");
        QTimer::singleShot(3000, this, &WifiManager::onScanResultsReady);
}

void WifiManager::onScanResultsReady()
{
        runWpaCli({"-i", mInterface, "scan_results"});

        disconnect(mProcess, nullptr, this, nullptr);
        connect(mProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this](int code, QProcess::ExitStatus) {
                if (code != 0) {
                        setStatus("Konnte Scan Ergebnisse nicht lesen");
                        mScanning = false;
                        emit scanningChanged();
                        clearProcess();
                        return;
                }
                QString output = QString::fromUtf8(mProcess->readAllStandardOutput());
                QStringList lines = output.split('\n', Qt::SkipEmptyParts);
                QStringList seen;
                for (const QString &line : lines) {
                        if (line.startsWith("bssid")) { continue; }
                        QStringList parts = line.split('\t');
                        if (parts.size() < 5) { continue; }
                        QString ssid = parts[4].trimmed();
                        if(ssid.isEmpty() || seen.contains(ssid)) { continue; }
                        seen.append(ssid);
                }
                mScanResults = seen;
                emit scanResultsChanged();
                mScanning = false;
                emit scanningChanged();
                setStatus(seen.isEmpty() ? "Keine Netzwerke gefunden" :
                          QString("%1 Netzwerk(e) gefunden").arg(seen.size()));
                clearProcess();
        });
}

void WifiManager::onConnectFinished(int exitCode)
{
        mConnecting = false;
        emit connectingChanged();
        setStatus(exitCode == 0 ? "Verbunden" : "Verbindung fehlgeschlagen");
        clearProcess();
}

void WifiManager::runWpaCli(const QStringList &args)
{
        clearProcess();
        mProcess = new QProcess(this);
        connect(mProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this](int code, QProcess::ExitStatus) {
                onScanFinished(code);
        });
        mProcess->start("wpa_cli", args);
}

void WifiManager::setStatus(const QString &msg)
{
        mStatusMessage = msg;
        emit statusMessageChanged();
}

void WifiManager::clearProcess()
{
        if (mProcess) {
                mProcess->deleteLater();
                mProcess = nullptr;
        }
}