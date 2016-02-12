QT                       += core
QT                       -= gui

TARGET                    = trost

CONFIG                   += console release
CONFIG                   -= app_bundle

TEMPLATE                  = app

INCLUDEPATH              += include

SOURCES                  += src/main.cpp

INSTALLS                 += target

QMAKE_CXXFLAGS_RELEASE   -= -O
QMAKE_CXXFLAGS_RELEASE   -= -O1
QMAKE_CXXFLAGS_RELEASE   -= -O2

linux {
  target.path             = /usr/local/bin
  QMAKE_CXXFLAGS         += -m64
  QMAKE_CXXFLAGS_RELEASE *= -O3
}

macx {
  target.path             = /usr/local/bin
  QMAKE_CXXFLAGS_RELEASE *= -O3
}
