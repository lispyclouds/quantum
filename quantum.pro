QT                       += core
QT                       -= gui

TARGET                    = quantum

CONFIG                   += console release
CONFIG                   += c++11
CONFIG                   -= app_bundle

TEMPLATE                  = app

LIBS                     += $$PWD/gc_compiled/lib/libgc.a

INCLUDEPATH              += include \
                            $$PWD/gc_compiled/include

SOURCES                  += src/vm.cpp \
                            src/main.cpp

INSTALLS                 += target

QMAKE_CXXFLAGS_RELEASE   -= -O
QMAKE_CXXFLAGS_RELEASE   -= -O1
QMAKE_CXXFLAGS_RELEASE   -= -O2
QMAKE_CFLAGS_RELEASE     -= -O2
QMAKE_POST_LINK           = ./make_installer.py

QMAKE_CLEAN              += installer/Qt* installer/quantum

linux {
  target.path             = /usr/local/bin
  QMAKE_CFLAGS_RELEASE   += -O3
  QMAKE_CXXFLAGS_RELEASE += -O3
}

macx {
  target.path             = /usr/local/bin
  QMAKE_CFLAGS_RELEASE   += -O3
  QMAKE_CFLAGS_RELEASE   += -Wno-unknown-attributes
  QMAKE_CXXFLAGS_RELEASE += -O3
  QMAKE_CXXFLAGS_RELEASE += -Wno-unknown-attributes
}
