TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    company.cpp \
    cli.cpp

HEADERS += \
    company.hh \
    cli.hh

DISTFILES += \
    data.csv
