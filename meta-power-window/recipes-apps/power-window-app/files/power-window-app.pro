QT += core gui widgets charts network
TARGET = power-window-app
TEMPLATE = app

HEADERS += BaseWindow.hpp PvStatsData.hpp KeyboardWidget.hpp SwipeManager.hpp \
           ScreenSaverManager.hpp WeatherWindow.hpp WeatherFetcher.hpp \
           SettingsWindow.hpp CurrentStatsWindow.hpp OverallStatsWindow.hpp \
           GrowattData.hpp GrowattFetcher.hpp PlantOverviewWindow.hpp
SOURCES += main.cpp SwipeManager.cpp ScreenSaverManager.cpp KeyboardWidget.cpp \
           WeatherWindow.cpp WeatherFetcher.cpp SettingsWindow.cpp \
           CurrentStatsWindow.cpp OverallStatsWindow.cpp GrowattFetcher.cpp \
           PlantOverviewWindow.cpp

LIBS += -lssl -lcrypto
