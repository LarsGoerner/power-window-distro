#ifndef BASEWINDOW_HPP_
#define BASEWINDOW_HPP_

#include <QWidget>

class BaseWindow : public QWidget
{
        Q_OBJECT
public:
        explicit BaseWindow(QWidget * parent = nullptr) : QWidget(parent)
        {
                // fill available space
                setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        }
        virtual ~BaseWindow() = default;
};

#endif // BASEWINDOW_HPP_