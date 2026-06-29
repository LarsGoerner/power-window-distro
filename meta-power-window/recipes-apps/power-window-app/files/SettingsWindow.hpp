#ifndef SETTINGSWINDOW_H_
#define SETTINGSWINDOW_H_

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
#include "BaseWindow.hpp"
#include "KeyboardWidget.hpp"
#include "GrowattFetcher.hpp"

class SettingsWindow : public BaseWindow
{
        Q_OBJECT
public:
        SettingsWindow(QWidget * parent = nullptr);
        void onBrightnessChanged(int value);
        void onScanClicked();
        void onScanResultsReady();
        void onWifiConnectClicked();
        void onUpdateClicked();
        void onGrowattConnectClicked();
        void onGrowattDemoClicked();
        void onGrowattClearClicked();
        QString growattUsername() const { return mGrowattUsernameInput->text(); }
        QString growattPassword() const { return mGrowattPasswordInput->text(); }
        QString growattServerUrl() const { return mGrowattServerCombo->currentData().toString(); }
        QString growattSelectedPlantId() const { return mGrowattPlantCombo->currentData().toString(); }
        void setGrowattStatus(const QString &text, const QString &color);
        void populateGrowattPlants(const QJsonArray &plants);

signals:
        void growattConnectRequested(const QString &serverUrl);
        void growattPlantSelected(const QString &plantId);

private:
        QTabBar * mTabBar;
        QStackedWidget * mSetViewContainer;
        KeyboardWidget * mKeyboard;

        QDial * mBrightnessDial;

        QComboBox * mSsidCombo;
        QLineEdit * mPasswordInput;
        QPushButton * mScanButton;
        QPushButton * mWifiConnectButton;
        QLabel * mWifiStatusLabel;
        QTimer * mScanTimer;
        QProcess * mWifiProcess;

        QPushButton * mUpdateButton;
        QTextEdit * mUpdateOutput;
        QTimer * mUpdatePollTimer;
        qint64 mUpdateOutputFilePos;

        QLineEdit * mGrowattUsernameInput;
        QLineEdit * mGrowattPasswordInput;
        QComboBox * mGrowattServerCombo;
        QPushButton * mGrowattConnectBtn;
        QPushButton * mGrowattClearBtn;
        QLabel * mGrowattStatusLabel;
        QComboBox * mGrowattPlantCombo;
};

#endif // SETTINGSWINDOW_H_