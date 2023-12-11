TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

LIBS += -L"C:\Games\GitWorks\KisaraguStation\KisaraguStation\SFML-2.3.2\lib"

CONFIG(release, debug|release):
LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system

CONFIG(debug, debug|release):
LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += "C:\Games\GitWorks\KisaraguStation\KisaraguStation\SFML-2.3.2\include"

SOURCES += main.cpp \
    classes.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    map.h
