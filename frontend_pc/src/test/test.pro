QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testloginchecker.cpp \
            ../app/login/loginchecker.cpp

HEADERS += ../app/login/loginchecker.h