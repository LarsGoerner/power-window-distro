#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

int main(int argc, char ** argv)
{
        QApplication app(argc, argv);

        QWidget window;
        window.setStyleSheet("background: white;");

        QVBoxLayout * layout = new QVBoxLayout;

        QLabel * label = new QLabel("Hello world!");
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("font-size: 48px; color: black; background: transparent;");

        QPushButton * btn1 = new QPushButton("Button 1");
        QPushButton * btn2 = new QPushButton("Button 2");

        layout->addWidget(label);
        layout->addWidget(btn1);
        layout->addWidget(btn2);

        window.setLayout(layout);
        window.showFullScreen();

        QObject::connect(btn1, &QPushButton::clicked, [label]() {
                label->setText("Button 1 pressed");
        });
        QObject::connect(btn2, &QPushButton::clicked, [label]() {
                label->setText("Button 2 pressed");
        });

        return app.exec();
}