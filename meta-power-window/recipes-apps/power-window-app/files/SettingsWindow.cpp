#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabBar>
#include <QStackedWidget>
#include <QLabel>
#include <QDial>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QJsonArray>
#include "BaseWindow.hpp"
#include "SettingsWindow.hpp"
#include "KeyboardWidget.hpp"
#include "GrowattFetcher.hpp"

SettingsWindow::SettingsWindow(QWidget * parent) : BaseWindow(parent)
{
        QVBoxLayout * mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(30);

        // TITLE
        QLabel * title = new QLabel("Einstellungen", this);
        title->setObjectName("windowTitle");
        title->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(title);

        // TAB BAR
        mTabBar = new QTabBar(this);
        mTabBar->addTab("Anzeige");
        mTabBar->addTab("WiFi");
        mTabBar->addTab("Growatt");
        mTabBar->addTab("Update");
        mainLayout->addWidget(mTabBar);

        // SETTING VIEWS CONTAINER
        mSetViewContainer = new QStackedWidget(this);

        // DISPLAY SETTING VIEW
        QWidget * displayView = new QWidget(this);
        QVBoxLayout * displayLayout = new QVBoxLayout(displayView);
        displayLayout->setContentsMargins(20, 20, 20, 20);
        displayLayout->setSpacing(15);

        QLabel * blBrightnessLabel = new QLabel("Helligkeit", displayView);
        blBrightnessLabel->setObjectName("sectionTitle");
        displayLayout->addWidget(blBrightnessLabel);

        mBrightnessDial = new QDial(displayView);
        mBrightnessDial->setRange(5, 100);
        mBrightnessDial->setValue(70);
        mBrightnessDial->setNotchesVisible(true);
        mBrightnessDial->setMinimumSize(400, 400);
        displayLayout->addWidget(mBrightnessDial, 1, Qt::AlignCenter);

        mSetViewContainer->addWidget(displayView);

        // WIFI CONFIG VIEW
        QWidget * wifiView = new QWidget(this);
        QVBoxLayout * wifiLayout = new QVBoxLayout(wifiView);

        QLabel * wifiLabel = new QLabel("WiFi Einstellungen", wifiView);
        wifiLabel->setStyleSheet("font-weight: bold;");
        wifiLayout->addWidget(wifiLabel);

        mScanButton = new QPushButton("Scannen", wifiView);
        mScanButton->setObjectName("scanButton");
        wifiLayout->addWidget(mScanButton);

        mSsidCombo = new QComboBox(wifiView);
        mSsidCombo->setObjectName("ssidCombo");
        mSsidCombo->setPlaceholderText("-- Netzwerk auswählen --");
        wifiLayout->addWidget(mSsidCombo);

        mPasswordInput = new QLineEdit(wifiView);
        mPasswordInput->setObjectName("passwordInput");
        mPasswordInput->setPlaceholderText("Passwort");
        mPasswordInput->setEchoMode(QLineEdit::Password);
        wifiLayout->addWidget(mPasswordInput);

        mWifiConnectButton = new QPushButton("Verbinden", wifiView);
        mWifiConnectButton->setObjectName("wifiConnectButton");
        wifiLayout->addWidget(mWifiConnectButton);

        mWifiStatusLabel = new QLabel("", wifiView);
        mWifiStatusLabel->setObjectName("wifiStatusLabel");
        wifiLayout->addWidget(mWifiStatusLabel);

        mSetViewContainer->addWidget(wifiView);

        // GROWATT SETTINGS VIEW
        QWidget * growattView = new QWidget(this);
        QVBoxLayout * growattLayout = new QVBoxLayout(growattView);

        QLabel * growattLabel = new QLabel("Growatt Konto", growattView);
        growattLayout->addWidget(growattLabel);

        mGrowattUsernameInput = new QLineEdit(growattView);
        mGrowattUsernameInput->setObjectName("growattUsernameInput");
        mGrowattUsernameInput->setPlaceholderText("Benutzername (E-Mail)");
        growattLayout->addWidget(mGrowattUsernameInput);

        mGrowattPasswordInput = new QLineEdit(growattView);
        mGrowattPasswordInput->setObjectName("growattPasswordInput");
        mGrowattPasswordInput->setPlaceholderText("Passwort");
        mGrowattPasswordInput->setEchoMode(QLineEdit::Password);
        growattLayout->addWidget(mGrowattPasswordInput);

        mGrowattServerCombo = new QComboBox(growattView);
        mGrowattServerCombo->setObjectName("growattServerCombo");
        mGrowattServerCombo->addItem("Europa / Welt (OpenAPI)", "https://openapi.growatt.com");
        mGrowattServerCombo->addItem("China (OpenAPI)", "https://openapi-cn.growatt.com");
        mGrowattServerCombo->addItem("Nordamerika (OpenAPI)", "https://openapi-us.growatt.com");
        mGrowattServerCombo->addItem("Australien / Neuseeland (OpenAPI)", "https://openapi-au.growatt.com");
        mGrowattServerCombo->addItem("server.growatt.com (Legacy)", "https://server.growatt.com");
        growattLayout->addWidget(mGrowattServerCombo);

        mGrowattPlantCombo = new QComboBox(growattView);
        mGrowattPlantCombo->setObjectName("growattPlantCombo");
        mGrowattPlantCombo->setPlaceholderText("-- Kraftwerk wählen --");
        growattLayout->addWidget(mGrowattPlantCombo);

        mGrowattStatusLabel = new QLabel("", growattView);
        mGrowattStatusLabel->setObjectName("growattStatusLabel");
        growattLayout->addWidget(mGrowattStatusLabel);

        QHBoxLayout * growattBtnRow = new QHBoxLayout(growattView);

        mGrowattConnectBtn = new QPushButton("Verbinden", growattView);
        mGrowattConnectBtn->setObjectName("growattConnectButton");
        growattBtnRow->addWidget(mGrowattConnectBtn);

        mGrowattClearBtn = new QPushButton("Löschen", growattView);
        mGrowattClearBtn->setObjectName("growattClearButton");
        growattBtnRow->addWidget(mGrowattClearBtn);

        growattLayout->addLayout(growattBtnRow);
        growattLayout->addStretch();
        mSetViewContainer->addWidget(growattView);

        // UPDATE VIEW
        QWidget * updateView = new QWidget(this);
        QVBoxLayout * updateLayout = new QVBoxLayout(updateView);

        QLabel * updateLabel = new QLabel("Software Update", updateView);
        updateLabel->setObjectName("updateLabel");
        updateLayout->addWidget(updateLabel);

        mUpdateButton = new QPushButton("Update starten", updateView);
        mUpdateButton->setObjectName("updateButton");
        updateLayout->addWidget(mUpdateButton);

        mUpdateOutput = new QTextEdit(updateView);
        mUpdateOutput->setReadOnly(true);
        mUpdateOutput->setObjectName("updateOutput");
        updateLayout->addWidget(mUpdateOutput, 1);

        updateLayout->addStretch();
        mSetViewContainer->addWidget(updateView);
        mainLayout->addWidget(mSetViewContainer);

        //KEYBOARD
        mKeyboard = new KeyboardWidget(this);
        mainLayout->addWidget(mKeyboard);
        mKeyboard->attachInput(mPasswordInput);
        mKeyboard->attachInput(mGrowattUsernameInput);
        mKeyboard->attachInput(mGrowattPasswordInput);

        connect(mTabBar, &QTabBar::currentChanged, mSetViewContainer, &QStackedWidget::setCurrentIndex);
        connect(mBrightnessDial, &QDial::valueChanged, this, &SettingsWindow::onBrightnessChanged);
        connect(mScanButton, &QPushButton::clicked, this, &SettingsWindow::onScanClicked);
        connect(mWifiConnectButton, &QPushButton::clicked, this, &SettingsWindow::onWifiConnectClicked);
        connect(mGrowattConnectBtn, &QPushButton::clicked, this, &SettingsWindow::onGrowattConnectClicked);
        connect(mGrowattClearBtn, &QPushButton::clicked, this, &SettingsWindow::onGrowattClearClicked);
        connect(mUpdateButton, &QPushButton::clicked, this, &SettingsWindow::onUpdateClicked);
}

void SettingsWindow::onBrightnessChanged(int value)
{
        qDebug() << "Setting backlight brightness to " << value << "%";
        QString backlightPath = "/sys/class/backlight/waveshare_bl/brightness";
        QFile blFile(backlightPath);
        if (blFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&blFile);
                int blValue = (value * 255) / 100;
                out << blValue;
                blFile.close();
        } else {
                qWarning() << "Failed to write to backlight file";
        }
}

void SettingsWindow::onScanClicked()
{
        mScanButton->setEnabled(false);
        mScanButton->setText("Scannen...");
        mSsidCombo->clear();
        mWifiStatusLabel->setText("");

        mScanTimer = new QTimer(this);
        mScanTimer->setSingleShot(true);
        connect(mScanTimer, &QTimer::timeout, this, &SettingsWindow::onScanResultsReady);

        mWifiProcess = new QProcess(this);
        connect(mWifiProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this](int code, QProcess::ExitStatus)
        {
                if (code == 0) { mScanTimer->start(3000); }
                else {
                        mWifiStatusLabel->setText("Scan fehlgeschlagen");
                        mWifiStatusLabel->setStyleSheet("Qlabel { background-color: red; color: white; }");
                        mScanButton->setEnabled(true);
                        mScanButton->setText("Scannen");
                }
        });
        mWifiProcess->start("wpa_cli", { "-i", "wlan0", "scan" });
}

void SettingsWindow::onScanResultsReady()
{
        mScanTimer->deleteLater();
        mScanTimer = nullptr;

        mWifiProcess = new QProcess(this);
        connect(mWifiProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this]() {
                QString output = QString::fromUtf8(mWifiProcess->readAllStandardOutput());
                QStringList lines = output.split('\n', Qt::SkipEmptyParts);
                QStringList seen;
                for (const QString &line : lines) {
                        if (line.startsWith("bssid")) { continue; }
                        QStringList parts = line.split('\t');
                        if (parts.size() < 5) { continue; }
                        QString ssid = parts[4].trimmed();
                        if (ssid.isEmpty() || seen.contains(ssid)) { continue; }
                        seen.append(ssid);
                        mSsidCombo->addItem(ssid);
                }
                mSsidCombo->insertItem(0, "-- Netzwerk auswählen --");
                mSsidCombo->setCurrentIndex(0);
                mScanButton->setEnabled(true);
                mScanButton->setText("Scannen");

                if (mSsidCombo->count() <= 1) {
                        mWifiStatusLabel->setText("Keine Netzwerke gefunden");
                        mWifiStatusLabel->setProperty("status", "warning");
                } else {
                        mWifiStatusLabel->setText("Scan erfolgreich");
                        mWifiStatusLabel->setProperty("status", "ok");
                }
                mWifiStatusLabel->style()->unpolish(mWifiStatusLabel);
                mWifiStatusLabel->style()->polish(mWifiStatusLabel);
                mWifiProcess->deleteLater();
                mWifiProcess = nullptr;
        });
        mWifiProcess->start("wpa_cli", { "-i", "wlan0", "scan_result" });
}

void SettingsWindow::onWifiConnectClicked()
{
        QString ssid = mSsidCombo->currentText();
        QString psk = mPasswordInput->text();
        if (ssid.isEmpty() || ssid.startsWith("--")) {
                mWifiStatusLabel->setText("Kein Netwerk ausgewählt");
                mWifiStatusLabel->setProperty("status", "warning");
                mWifiStatusLabel->style()->unpolish(mWifiStatusLabel);
                mWifiStatusLabel->style()->polish(mWifiStatusLabel);
                return;
        }
        if (psk.isEmpty()) {
                mWifiStatusLabel->setText("Kein Passwork eingegeben");
                mWifiStatusLabel->setProperty("status", "warning");
                mWifiStatusLabel->style()->unpolish(mWifiStatusLabel);
                mWifiStatusLabel->style()->polish(mWifiStatusLabel);
                return;
        }

        mWifiConnectButton->setEnabled(false);
        mWifiConnectButton->setText("Verbinde...");
        mWifiStatusLabel->setText("");

        QString cmd = QString(
                "HASH=$(wpa_passphrase \"%1\" \"%2\" | grep -o 'psk=[0-9a-f]\\{64\\}' | cut -d= -f2) &&"
                "wpa_cli -i wlan0 remove_network 0 2>/dev/null; "
                "NETID=$(wpa_cli -i wlan0 add_network | tail -1) && "
                "wpa_cli -i wlan0 set_network $NETID ssid '\"%1\"' && "
                "wpa_cli -i wlan0 set_network $NETID psk $HASH && "
                "wpa_cli -i wlan0 enable_network $NETID && "
                "wpa_cli -i wlan0 save_config"
        ).arg(ssid, psk);

        mWifiProcess = new QProcess(this);
        connect(mWifiProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this](int code, QProcess::ExitStatus)
        {
                if (code == 0) {
                        mWifiStatusLabel->setText("Verbunden");
                        mWifiStatusLabel->setProperty("status", "ok");
                } else {
                        mWifiStatusLabel->setText("Verbindung fehlgeschlagen");
                        mWifiStatusLabel->setProperty("status", "error");
                }
                mWifiStatusLabel->style()->unpolish(mWifiStatusLabel);
                mWifiStatusLabel->style()->polish(mWifiStatusLabel);
                mWifiConnectButton->setEnabled(true);
                mWifiConnectButton->setText("Verbinden");
                mWifiProcess->deleteLater();
                mWifiProcess = nullptr;
        });
        mWifiProcess->start("sh", { "-c", cmd });
}

void SettingsWindow::onUpdateClicked()
{
        mUpdateButton->setEnabled(false);
        mUpdateButton->setText("Update läuft...");
        mUpdateOutput->clear();

        // write update script to /tmp
        QFile sf("/tmp/ota-update.sh");
        if (sf.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                QTextStream s(&sf);
                s << "#!/bin/sh\n";
                s << "opkg update && opkg upgrade\n";
                s << "echo \"=== EXIT CODE $? ===\"\n";
                sf.close();
        }
        QFile::setPermissions("/tmp/ota-update.sh", QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
        QFile::remove("/tmp/ota-update.log");

        // launch in separate systemd scope
        QProcess::startDetached("systemd-run", {
                "--scope", "--unit=power-window-ota", "--collect",
                "sh", "-c", "/tmp/ota-update.sh > /tmp/ota-update.log 2>&1"
        });

        // poll log
        mUpdateOutputFilePos = 0;
        mUpdatePollTimer = new QTimer(this);
        connect(mUpdatePollTimer, &QTimer::timeout, this, [this]() {
                QFile f("/tmp/ota-update.log");
                if (f.open(QIODevice::ReadOnly)) {
                        if (f.size() > mUpdateOutputFilePos) {
                                f.seek(mUpdateOutputFilePos);
                                QByteArray data = f.readAll();
                                mUpdateOutput->append(QString::fromUtf8(data));
                                mUpdateOutputFilePos = f.pos();

                                // check if update finished
                                if (data.contains("=== EXIT CODE")) {
                                        f.close();
                                        mUpdatePollTimer->stop();
                                        mUpdatePollTimer->deleteLater();
                                        mUpdatePollTimer = nullptr;
                                        mUpdateButton->setEnabled(true);
                                        mUpdateButton->setText("Update starten");
                                        return;
                                }
                        }
                        f.close();
                }
        });
        mUpdatePollTimer->start(500);
        mUpdateOutput->append("Update wirk im Hintergrund ausgeführt...\n");
}

void SettingsWindow::onGrowattConnectClicked()
{
        emit growattConnectRequested(mGrowattServerCombo->currentData().toString());
}

void SettingsWindow::onGrowattClearClicked()
{
        mGrowattUsernameInput->clear();
        mGrowattPasswordInput->clear();
        mGrowattPlantCombo->clear();
        mGrowattStatusLabel->setText("Zugangsdaten gelöscht");
        mGrowattStatusLabel->setProperty("status", "warning");
        mGrowattStatusLabel->style()->unpolish(mGrowattStatusLabel);
        mGrowattStatusLabel->style()->polish(mGrowattStatusLabel);
}

void SettingsWindow::setGrowattStatus(const QString &text, ConnectionStatus status)
{
        mGrowattStatusLabel->setText(text);

        switch (status) {
        case ConnectionStatus::OK:
                mGrowattStatusLabel->setProperty("status", "ok");
                break;
        case ConnectionStatus::WARNING:
                mGrowattStatusLabel->setProperty("status", "ok");
                break;
        case ConnectionStatus::ERROR:
                mGrowattStatusLabel->setProperty("status", "ok");
                break;
        default:
                break;
        }
        mGrowattStatusLabel->style()->unpolish(mGrowattStatusLabel);
        mGrowattStatusLabel->style()->polish(mGrowattStatusLabel);
}

void SettingsWindow::populateGrowattPlants(const QJsonArray &plants)
{
        mGrowattPlantCombo->clear();
        for (const QJsonValue &val : plants) {
                QJsonObject obj = val.toObject();
                mGrowattPlantCombo->addItem(obj["plantName"].toString(), obj["plantId"].toString());
        }
}
