QT += widgets

CONFIG += c++11
QMAKE_CXXFLAGS += -g -W -Wall -Werror -Wfloat-equal -Wunused -Wnon-virtual-dtor -isystem $$[QT_INSTALL_HEADERS]

HEADERS = common_defs.h graph_widget.h
SOURCES = main.cpp graph_widget.cpp
