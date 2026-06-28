#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "SwipeManager.hpp"
#include "BaseWindow.hpp"

class CustomPage : public BaseWindow
{
public:
        CustomPage(const QString & title, const QString & bgColor, QWidget * parent = nullptr) : BaseWindow(parent)
        {
                setStyleSheet(QString("background-color: %1;").arg(bgColor));

                QVBoxLayout * layout = new QVBoxLayout(this);
                QLabel * label = new QLabel(title, this);
                label->setAlignment(Qt::AlignCenter);
                label->setStyleSheet("font-size: 30px; color: white; font-weight: bold;");

                QPushButton * btn = new QPushButton("Click Me", this);
                btn->setFixedSize(150, 50);

                layout->addWidget(label);
                layout->addWidget(btn, 0, Qt::AlignCenter);
        }
};

int main(int argc, char ** argv)
{
        QApplication app(argc, argv);

        SwipeManager sm;

        CustomPage * page1 = new CustomPage("Window 1", "#34495e");
        CustomPage * page2 = new CustomPage("Window 2", "#2ecc71");
        CustomPage * page3 = new CustomPage("Window 3", "#e74c3c");

        sm.addWindow(page1);
        sm.addWindow(page2);
        sm.addWindow(page3);

        sm.showFullScreen();
        return app.exec();
}
