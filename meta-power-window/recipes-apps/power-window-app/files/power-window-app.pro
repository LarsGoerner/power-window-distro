QT += qml quick network charts widgets

CONFIG += c++11

SOURCES += \
        src/main.cpp \
        src/Theme.cpp \
        src/Backlight.cpp \
        src/Updater.cpp \
        src/WifiManager.cpp \
        src/DhtSensor.cpp \
        src/WeatherFetcher.cpp

HEADERS += \
        src/Theme.hpp \
        src/Backlight.hpp \
        src/Updater.hpp \
        src/WifiManager.hpp \
        src/DhtSensor.hpp \
        src/WeatherFetcher.hpp

RESOURCES += resources.qrc

target.path = /usr/bin
INSTALLS += target