QT += testlib
QT += network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testloginchecker.cpp \
            ../app/Login/loginchecker.cpp \
            ../app/commu/communhttp.cpp \
            ../app/commu/patient.cpp \
            ../app/commu/userinfo.cpp \
            tst_all.cpp \
            tst_testcommun.cpp

HEADERS += ../app/Login/loginchecker.h \
    ../app/commu/communhttp.h \
    ../app/commu/patient.h \
    ../app/commu/urlbase.h \
    ../app/commu/userinfo.h \
    tst_testcommun.h \
    tst_testloginchecker.h
