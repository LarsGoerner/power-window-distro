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
        title->setStyleSheet("font-size: 28px; font-weight: bold;");
        title->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(title);

        // TAB BAR
        mTabBar = new QTabBar(this);
        mTabBar->addTab("Anzeige");
        mTabBar->addTab("WiFi");
        mTabBar->addTab("Growatt");
        mTabBar->addTab("Update");
        mTabBar->setStyleSheet("QTabBar::tab { height: 40px; padding: 0px 20px; }");
        mainLayout->addWidget(mTabBar);

        // SETTING VIEWS CONTAINER
        mSetViewContainer = new QStackedWidget(this);

        // DISPLAY SETTING VIEW
        QWidget * displayView = new QWidget(this);
        QVBoxLayout * displayLayout = new QVBoxLayout(displayView);
        displayLayout->setContentsMargins(20, 20, 20, 20);
        displayLayout->setSpacing(15);

        QLabel * blBrightnessLabel = new QLabel("Helligkeit", displayView);
        blBrightnessLabel->setStyleSheet("font-weight: bold;");
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
        wifiLayout->setContentsMargins(20, 20, 20, 20);
        wifiLayout->setSpacing(15);

        QLabel * wifiLabel = new QLabel("WiFi Einstellungen", wifiView);
        wifiLabel->setStyleSheet("font-weight: bold;");
        wifiLayout->addWidget(wifiLabel);

        mScanButton = new QPushButton("Scannen", wifiView);
        mScanButton->setFixedHeight(45);
        mScanButton->setStyleSheet("background-color: green; font-weight: bold; border-radius: 5px;");
        wifiLayout->addWidget(mScanButton);

        mSsidCombo = new QComboBox(wifiView);
        mSsidCombo->setFixedHeight(45);
        mSsidCombo->setPlaceholderText("-- Netzwerk auswählen --");
        mSsidCombo->setStyleSheet("background-color: green; font-weight: bold; border-radius: 5px;");
        wifiLayout->addWidget(mSsidCombo);

        mPasswordInput = new QLineEdit(wifiView);
        mPasswordInput->setPlaceholderText("Passwort");
        mPasswordInput->setEchoMode(QLineEdit::Password);
        mPasswordInput->setFixedHeight(45);
        mPasswordInput->setStyleSheet("background-color: darkBlue; border: 1px solid gray; padding-left: 10px; border-radius: 5px;");
        wifiLayout->addWidget(mPasswordInput);

        mWifiConnectButton = new QPushButton("Verbinden", wifiView);
        mWifiConnectButton->setFixedHeight(45);
        mWifiConnectButton->setStyleSheet("background-color: green; font-weight: bold; border-radius: 5px;");
        wifiLayout->addWidget(mWifiConnectButton);

        mWifiStatusLabel = new QLabel("", wifiView);
        mWifiStatusLabel->setFixedHeight(45);
        mWifiStatusLabel->setStyleSheet("background-color: green; font-weight: bold; border-radius: 5px;");
        wifiLayout->addWidget(mWifiStatusLabel);

        mSetViewContainer->addWidget(wifiView);

        // GROWATT SETTINGS VIEW
        QWidget * growattView = new QWidget(this);
        QVBoxLayout * growattLayout = new QVBoxLayout(growattView);
        growattLayout->setContentsMargins(20, 20, 20, 20);
        growattLayout->setSpacing(12);

        QLabel * growattLabel = new QLabel("Growatt Konto", growattView);
        growattLayout->addWidget(growattLabel);

        mGrowattUsernameInput = new QLineEdit(growattView);
        mGrowattUsernameInput->setPlaceholderText("Benutzername (E-Mail)");
        mGrowattUsernameInput->setFixedHeight(45);
        growattLayout->addWidget(mGrowattUsernameInput);

        mGrowattPasswordInput = new QLineEdit(growattView);
        mGrowattPasswordInput->setPlaceholderText("Passwort");
        mGrowattPasswordInput->setEchoMode(QLineEdit::Password);
        mGrowattPasswordInput->setFixedHeight(45);
        growattLayout->addWidget(mGrowattPasswordInput);

        mGrowattServerCombo = new QComboBox(growattView);
        mGrowattServerCombo->addItem("Europa / Welt (OpenAPI)", "https://openapi.growatt.com");
        mGrowattServerCombo->addItem("China (OpenAPI)", "https://openapi-cn.growatt.com");
        mGrowattServerCombo->addItem("Nordamerika (OpenAPI)", "https://openapi-us.growatt.com");
        mGrowattServerCombo->addItem("Australien / Neuseeland (OpenAPI)", "https://openapi-au.growatt.com");
        mGrowattServerCombo->addItem("server.growatt.com (Legacy)", "https://server.growatt.com");
        growattLayout->addWidget(mGrowattServerCombo);

        mGrowattPlantCombo = new QComboBox(growattView);
        mGrowattPlantCombo->setPlaceholderText("-- Kraftwerk wählen --");
        growattLayout->addWidget(mGrowattPlantCombo);

        mGrowattStatusLabel = new QLabel("", growattView);
        mGrowattStatusLabel->setFixedHeight(45);
        growattLayout->addWidget(mGrowattStatusLabel);

        QHBoxLayout * growattBtnRow = new QHBoxLayout(growattView);

        mGrowattConnectBtn = new QPushButton("Verbinden", growattView);
        mGrowattConnectBtn->setFixedHeight(45);
        growattBtnRow->addWidget(mGrowattConnectBtn);

        mGrowattClearBtn = new QPushButton("Löschen", growattView);
        mGrowattClearBtn->setFixedHeight(45);
        growattBtnRow->addWidget(mGrowattClearBtn);

        growattLayout->addLayout(growattBtnRow);
        growattLayout->addStretch();
        mSetViewContainer->addWidget(growattView);

        // UPDATE VIEW
        QWidget * updateView = new QWidget(this);
        QVBoxLayout * updateLayout = new QVBoxLayout(updateView);
        updateLayout->setContentsMargins(20, 20, 20, 20);
        updateLayout->setSpacing(15);

        QLabel * updateLabel = new QLabel("Software Update", updateView);
        updateLabel->setStyleSheet("font-weight: bold;");
        updateLayout->addWidget(updateLabel);

        mUpdateButton = new QPushButton("Update starten", updateView);
        mUpdateButton->setFixedHeight(45);
        mUpdateButton->setStyleSheet("background-color: green; font-weight: bold; border-radius: 5px;");
        updateLayout->addWidget(mUpdateButton);

        mUpdateOutput = new QTextEdit(updateView);
        mUpdateOutput->setReadOnly(true);
        mUpdateOutput->setStyleSheet("background-color: darkBlue, color: green; font-family: monospace; border: 1px solid gray; border-radius: 5px; padding: 10px");
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
                        mWifiStatusLabel->setStyleSheet("Qlabel { background-color: yellow; color: black; }");
                } else {
                        mWifiStatusLabel->setText("Scan erfolgreich");
                        mWifiStatusLabel->setStyleSheet("Qlabel { background-color: green; color: black; }");
                }
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
                mWifiStatusLabel->setStyleSheet("QLabel { background-color: yellow; color: black; }");
                return;
        }
        if (psk.isEmpty()) {
                mWifiStatusLabel->setText("Kein Passwork eingegeben");
                mWifiStatusLabel->setStyleSheet("QLabel { background-color: yellow; color: black; }");
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
                        mWifiStatusLabel->setStyleSheet("QLabel { background-color: green; color: black; }");
                } else {
                        mWifiStatusLabel->setText("Verbindung fehlgeschlagen");
                        mWifiStatusLabel->setStyleSheet("QLabel { background-color: red; color: white; }");
                }
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
        mGrowattStatusLabel->setStyleSheet(
                "QLabel {background-color: yellow; color: black; border-radius: 5px;}");
}

void SettingsWindow::setGrowattStatus(const QString &text, const QString &color)
{
        mGrowattStatusLabel->setText(text);
        mGrowattStatusLabel->setStyleSheet(
                QString("QLabel { background-color: %1; color: black; border-radius: 5px; }")
                .arg(color));
}

void SettingsWindow::populateGrowattPlants(const QJsonArray &plants)
{
        mGrowattPlantCombo->clear();
        for (const QJsonValue &val : plants) {
                QJsonObject obj = val.toObject();
                mGrowattPlantCombo->addItem(obj["plantName"].toString(), obj["plantId"].toString());
        }
}
