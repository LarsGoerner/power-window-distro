#ifndef SCREEN_SAVER_MANAGER_HPP_
#define SCREEN_SAVER_MANAGER_HPP_

#include <QObject>
#include <QTimer>
#include <QEvent>

class ScreenSaverManager : public QObject
{
        Q_OBJECT
public:
        explicit ScreenSaverManager(int idleTimeoutSecs = 30, QObject * parent = nullptr);

private:
        QTimer * mIdleTimer;
        int mTimeout;
        int mLastBrightness;
        bool mDisplayBlanked = false;

        bool eventFilter(QObject * obj, QEvent * event) override;
        void blankDisplay(bool blank);
};

#endif // SCREEN_SAVER_MANAGER_HPP_