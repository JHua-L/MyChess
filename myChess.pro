#-------------------------------------------------
#
# Project created by QtCreator 2020-01-29T16:47:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myChess
TEMPLATE = app


SOURCES += main.cpp\
        chess.cpp \
    chessform.cpp

HEADERS  += chess.h \
    chessform.h

FORMS += \
    chessform.ui

CONFIG += c++11
