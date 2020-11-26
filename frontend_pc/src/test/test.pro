QT += testlib
QT += network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  test_login.cpp \
            ../app/Login/loginchecker.cpp \
            ../app/commu/communhttp.cpp \
            ../app/commu/patient.cpp \
            ../app/commu/userinfo.cpp \
            test_main.cpp \
            test_commun.cpp \
            test_patient.cpp

HEADERS += ../app/Login/loginchecker.h \
    ../app/commu/communhttp.h \
    ../app/commu/patient.h \
    ../app/commu/urlbase.h \
    ../app/commu/userinfo.h \
    test_commun.h \
    test_login.h \
    test_patient.h

