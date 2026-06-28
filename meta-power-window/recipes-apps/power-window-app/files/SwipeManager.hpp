#ifndef SWIPEMANAGER_HPP_
#define SWIPEMANAGER_HPP_

#include <QStackedWidget>
#include "BaseWindow.hpp"

class SwipeManager : public QStackedWidget
{
        Q_OBJECT
public:
        explicit SwipeManager(QWidget * parent = nullptr);
        void addWindow(BaseWindow * wnd);
        bool eventFilter(QObject * obj, QEvent * event) override;

private:
        enum class SlideDirection { Left, Right };

        QPoint mTouchStartGlobal;
        int mActivePage;
        bool mIsAnimating;

        void slideInPage(int nextIdx, SlideDirection dir);
};

#endif // SWIPEMANAGER_HPP_