#ifndef SWIPEMANAGER_HPP_
#define SWIPEMANAGER_HPP_

#include <QStackedWidget>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QPoint>
#include "BaseWindow.hpp"

class SwipeManager : public QStackedWidget
{
        Q_OBJECT
public:
        explicit SwipeManager(QWidget * parent = nullptr)
                : QStackedWidget(parent), mActivePage(0), mIsAnimating(false) {}
        void addWindow(BaseWindow * wnd) { this->addWidget(wnd); }

protected:
        void mousePressEvent(QMouseEvent * event) override
        {
                if (mIsAnimating) { return; }
                mTouchStartPos = event->pos();
        }
        void mouseReleaseEvent(QMouseEvent * event) override
        {
                if (mIsAnimating) { return; }
                QPoint delta = event->pos() - mTouchStartPos;
                if (qAbs(delta.x()) > 100) {
                        if (delta.x() < 0) {
                                slideInPage(mActivePage + 1, SlideDirection::Left);
                        } else {
                                slideInPage(mActivePage - 1, SlideDirection::Right);
                        }
                }
        }

private:
        enum class SlideDirection { Left, Right };

        QPoint mTouchStartPos;
        int mActivePage;
        bool mIsAnimating;

        void slideInPage(int nextIdx, SlideDirection dir)
        {
                if (nextIdx < 0 || nextIdx >= count() || mIsAnimating) return;

                mIsAnimating = true;
                QWidget * currentWidget = widget(mActivePage);
                QWidget * nextWidget = widget(nextIdx);

                int width = this->width();

                int startX = (dir == SlideDirection::Left) ? width : -width;
                nextWidget->setGeometry(startX, 0, width, this->height());
                nextWidget->show();
                nextWidget->raise();

                QPropertyAnimation * animCurrent = new QPropertyAnimation(currentWidget, "pos");
                QPropertyAnimation * animNext = new QPropertyAnimation(nextWidget, "pos");

                animCurrent->setDuration(400);
                animCurrent->setEasingCurve(QEasingCurve::OutQuint);
                animCurrent->setStartValue(QPoint(0, 0));
                animCurrent->setEndValue(QPoint(-startX, 0));

                animNext->setDuration(400);
                animNext->setEasingCurve(QEasingCurve::OutQuint);
                animNext->setStartValue(QPoint(startX, 0));
                animNext->setEndValue(QPoint(0, 0));

                connect(animNext, &QPropertyAnimation::finished, this, [this, nextIdx, currentWidget]() {
                        currentWidget->hide();
                        setCurrentIndex(nextIdx);
                        mActivePage = nextIdx;
                        mIsAnimating = false;
                });

                animCurrent->start(QAbstractAnimation::DeleteWhenStopped);
                animNext->start(QAbstractAnimation::DeleteWhenStopped);
        }
};

#endif // SWIPEMANAGER_HPP_