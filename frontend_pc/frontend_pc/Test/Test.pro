QT += testlib
QT += network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testloginchecker.cpp \
            ../App/Login/loginchecker.cpp \
            ../App/commu/communhttp.cpp \
            ../App/commu/patient.cpp \
            ../App/commu/userinfo.cpp \
            tst_all.cpp \
            tst_testcommun.cpp

HEADERS += ../App/Login/loginchecker.h \
    ../App/commu/communhttp.h \
    ../App/commu/patient.h \
    ../App/commu/urlbase.h \
    ../App/commu/userinfo.h
