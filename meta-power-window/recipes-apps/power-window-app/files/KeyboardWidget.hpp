#ifndef KEYBOARD_WIDGET_HPP_
#define KEYBOARD_WIDGET_HPP_

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class KeyboardWidget : public QWidget
{
        Q_OBJECT
public:
        explicit KeyboardWidget(QWidget * parent = nullptr);
        void attachInput(QLineEdit * input);

private:
        QLineEdit * mAttachedInput = nullptr;
        QList<QPushButton *> mKeyButtons;
        QStringList mKeys;
        bool mShift = false;
        bool mNumericMode = false;
        bool mSpecialCharsMode = false;
        const QStringList mKeyRow1Default { "q", "w", "e", "r", "t", "z", "u", "i", "o", "p" };
        const QStringList mKeyRow1Numeric { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };
        const QStringList mKeyRow1Special { "q", "w", "e", "r", "t", "z", "ü", "i", "ö", "p" };
        const QStringList mKeyRow2Default { "a", "s", "d", "f", "g", "h", "j", "k", "l" };
        const QStringList mKeyRow2Special { "ä", "ß", "d", "f", "g", "h", "j", "k", "l" };
        const QStringList mKeyRow3Default { "SHIFT", "y", "x", "c", "v", "b", "n", "m", "BACK" };
        const QStringList mKeyRow4Default { "[123]", "[ÄÜÖ]", "SPACE", "OK" };

        QPushButton * makeKey(int index);
        void updateLabels();
        void setKeyText(int idx, const QString & text);
        void onKeyClicked(int index);
        void onShiftClicked();
        void onBackspaceClicked();
        void onDoneClicked();
        void onFocusChanged(QWidget * old, QWidget * now);
};

#endif // KEYBOARD_WIDGET_HPP_