TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L"C:\Users\user\Documents\MyQtSFML\SFML-2.5.1\lib"

CONFIG(release, debug|release):
LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system

CONFIG(debug, debug|release):
LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += "C:\Users\user\Documents\MyQtSFML\SFML-2.5.1\include"

SOURCES += main.cpp \
    classes.cpp

include(DEPLOYMENT.pri)
qtcAddDeployment()

HEADERS += \
    map.h
