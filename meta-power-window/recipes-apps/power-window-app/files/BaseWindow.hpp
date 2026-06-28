#ifndef BASEWINDOW_HPP_
#define BASEWINDOW_HPP_

#include <QWidget>

class BaseWindow : public QWidget
{
        Q_OBJECT
public:
        explicit BaseWindow(QWidget * parent = nullptr) : QWidget(parent)
        {
                setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                setStyleSheet("background-color: darkBlue; color: white; font-size: 18px");
        }
        virtual ~BaseWindow() = default;
};

#endif // BASEWINDOW_HPP_