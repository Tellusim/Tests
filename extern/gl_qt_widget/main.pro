# Tellusim header

TARGET = main
TEMPLATE = app

DESTDIR = ./
MOC_DIR = build
OBJECTS_DIR = build

CONFIG += c++11 console debug
QT += core gui widgets opengl

INCLUDEPATH += ../../../include

win32 {
	exists(../../../source/Tellusim_x64d.lib) { LIBS += ../../../source/Tellusim_x64d.lib }
	else { LIBS += ../../../lib/windows/x64/Tellusim_x64d.lib }
}
unix {
	LIBS += -L../../../lib/linux/x64 -L../../../source -lTellusim_x64d
	QMAKE_LFLAGS += -Wl,-rpath,../../../lib/linux/x64
}

HEADERS += QGLWidget.h
SOURCES += QGLWidget.cpp main.cpp
