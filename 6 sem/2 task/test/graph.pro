QT += widgets

CONFIG += c++11
QMAKE_CXXFLAGS += -g -W -Wall -Werror -Wfloat-equal -Wunused -Wnon-virtual-dtor -isystem $$[QT_INSTALL_HEADERS]

OBJECTS_DIR = build/obj
MOC_DIR     = build/moc
DESTDIR     = build/bin

MAIN_H = CommonDefs.h MainWindow.h
MAIN_S = main.cpp MainWindow.cpp

DRAW_H = DrawArea.h
DRAW_S = DrawArea.cpp

RENDER_H = "3DRender.h" Geometry.h
RENDER_S = "3DRender.cpp" Geometry.cpp

EXTRA_DEBUG = trapfpe_gcc.c

HEADERS += \
		$$MAIN_H \
		$$DRAW_H \
		$$RENDER_H

SOURCES += \
		$$MAIN_S \
		$$DRAW_S \
		$$RENDER_S \
		$$EXTRA_DEBUG


