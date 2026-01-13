TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
SOURCES += functions.cpp
SOURCES += tests.cpp

HEADERS += functions.h
HEADERS += tests.h
HEADERS += typeDef.h

DISTFILES += \
    smallDataset.txt

