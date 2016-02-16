QT                       += core
QT                       -= gui

TARGET                    = quantum

CONFIG                   += console release
CONFIG                   -= app_bundle

TEMPLATE                  = app

INCLUDEPATH              += include

SOURCES                  += src/core/valuemanip.cpp \
                            src/vm.cpp \
                            src/main.cpp

INSTALLS                 += target

QMAKE_CXXFLAGS_RELEASE   -= -O
QMAKE_CXXFLAGS_RELEASE   -= -O1
QMAKE_CXXFLAGS_RELEASE   -= -O2
QMAKE_CFLAGS_RELEASE     -= -O2

linux {
  target.path            = /usr/local/bin
  QMAKE_CFLAGS_RELEASE   += -O3
  QMAKE_CXXFLAGS_RELEASE += -O3
}

macx {
  target.path            = /usr/local/bin
  QMAKE_CFLAGS_RELEASE   += -O3
  QMAKE_CXXFLAGS_RELEASE += -O3
}
