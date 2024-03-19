# Tellusim header

TARGET = main
TEMPLATE = app

DESTDIR = ./
MOC_DIR = build
OBJECTS_DIR = build

CONFIG += c++11 console debug
QT += core gui widgets

INCLUDEPATH += ../../../include

exists(../../../source/Tellusim_x64d.lib) { LIBS += ../../../source/Tellusim_x64d.lib }
else { LIBS += ../../../lib/windows/x64/Tellusim_x64d.lib }
LIBS += dxgi.lib d3d11.lib

HEADERS += QD3D11Widget.h
SOURCES += QD3D11Widget.cpp main.cpp
