# Tellusim header

TARGET = main
TEMPLATE = app

DESTDIR = ./
MOC_DIR = build
OBJECTS_DIR = build

CONFIG += c++11 console debug
QT += core gui widgets

QMAKE_CXXFLAGS += -DTS_DEBUG=1

INCLUDEPATH += ../../../include
INCLUDEPATH += $$(VULKAN_SDK)/include

win32 {
	exists(../../../source/Tellusim_x64d.lib) { LIBS += ../../../source/Tellusim_x64d.lib }
	else { LIBS += ../../../lib/windows/x64/Tellusim_x64d.lib }
	LIBS += $$(VULKAN_SDK)/lib/vulkan-1.lib
}
unix {
	LIBS += -L../../../lib/linux/x64 -L../../../source -lTellusim_x64d
	LIBS += -L$$(VULKAN_SDK)/lib -lvulkan -lX11
	QMAKE_LFLAGS += -Wl,-rpath,../../../lib/linux/x64
	QMAKE_CXXFLAGS += -Wno-deprecated-copy
}

HEADERS += QVKWidget.h
SOURCES += QVKWidget.cpp main.cpp
