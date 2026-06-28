#include <QStackedWidget>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QPoint>
#include <QApplication>
#include "SwipeManager.hpp"

SwipeManager::SwipeManager(QWidget * parent) : QStackedWidget(parent), mActivePage(0), mIsAnimating(false)
{
        qApp->installEventFilter(this);
}

void SwipeManager::addWindow(BaseWindow * wnd)
{
        this->addWidget(wnd);
        wnd->installEventFilter(this);
}

bool SwipeManager::eventFilter(QObject * obj, QEvent * event)
{
        if (mIsAnimating) { return QStackedWidget::eventFilter(obj, event); }
        if (event->type() == QEvent::MouseButtonPress) {
                QWidget * src = qobject_cast<QWidget *>(obj);
                if (src && this->isAncestorOf(src)) {
                        mTouchStartGlobal = static_cast<QMouseEvent *>(event)->globalPos();
                }
        } else if (event->type() == QEvent::MouseButtonRelease) {
                QWidget * src = qobject_cast<QWidget *>(obj);
                if (src && this->isAncestorOf(src)) {
                        QPoint releaseGlobal = static_cast<QMouseEvent *>(event)->globalPos();
                        int deltaX = releaseGlobal.x() - mTouchStartGlobal.x();
                        if(qAbs(deltaX) > 100) {
                                if (deltaX < 0) { slideInPage(mActivePage + 1, SlideDirection::Left); }
                                else { slideInPage(mActivePage - 1, SlideDirection::Right); }
                                return true;
                        }
                }
        }
        return QStackedWidget::eventFilter(obj, event);
}

void SwipeManager::slideInPage(int nextIdx, SlideDirection dir)
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