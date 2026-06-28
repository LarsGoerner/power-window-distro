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

private:
        QTabBar * mTabBar;
        QStackedWidget * mSetViewContainer;

        QDial * mBrightnessDial;

        QComboBox * mSsidCombo;
        QLineEdit * mPasswordInput;
        QPushButton * mScanButton;
        QPushButton * mConnectButton;
        QLabel * mWifiStatusLabel;
        QTimer * mScanTimer;
        QProcess * mWifiProcess;
        KeyboardWidget * mKeyboard;

        QPushButton * mUpdateButton;
        QTextEdit * mUpdateOutput;
        QTimer * mUpdatePollTimer;
        qint64 mUpdateOutputFilePos;
};

#endif // SETTINGSWINDOW_H_