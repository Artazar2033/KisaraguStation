TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

LIBS += -L"D:\QT CREATOR\KisaraguStation\SFML-2.3.2\lib"

CONFIG(release, debug|release):
LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system

CONFIG(debug, debug|release):
LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += "D:\QT CREATOR\KisaraguStation\SFML-2.3.2\include"

SOURCES += main.cpp \
    entity.cpp \
    bullet.cpp \
    enemy.cpp \
    map.cpp \
    player.cpp \
    vendingmachine.cpp \
    ending.cpp\
    deathanimation.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += map.h \
    arrmaps.h \
    bullet.h \
    enemy.h \
    entity.h \
    libs.h \
    player.h \
    vendingmachine.h \
    coin.h \
    ending.h\
    deathanimation.h \
    game.h
