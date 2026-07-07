QT += qml quick

CONFIG += c++11

SOURCES += main.cpp Theme.cpp Backlight.cpp Updater.cpp WifiManager.cpp
HEADERS += Theme.hpp Backlight.hpp Updater.hpp WifiManager.hpp

RESOURCES += resources.qrc

target.path = /usr/bin
INSTALLS += target