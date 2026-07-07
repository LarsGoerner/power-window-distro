#ifndef SCREEN_SAVER_HPP_
#define SCREEN_SAVER_HPP_

#include <QObject>
#include <QTimer>

class Backlight;

class ScreenSaver : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QString mode READ mode WRITE setMode NOTIFY modeChanged)
        Q_PROPERTY(int idleTimeoutSecs READ idleTimeoutSecs CONSTANT)

public:
        explicit ScreenSaver(Backlight * backlight, QObject * parent = nullptr);
        QString mode() const;
        int idleTimeoutSecs() const;
        void setMode(const QString &mode);

signals:
        void modeChanged();

private slots:
        void onIdleTimeout();

private:
        Backlight * mBacklight;
        QTimer * mIdleTimer;
        QString mMode;
        int mSavedBrightness;
        bool mIsActive;

        bool eventFilter(QObject * obj, QEvent * evt) override;
        void setDisplayBlank(bool blank);
};

#endif // SCREEN_SAVER_HPP_