#ifndef TEST_PATIENT_H
#define TEST_PATIENT_H

#include <QTest>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QVector>

#include "../app/commu/communhttp.h"
#include "../app/commu/userinfo.h"
#include "../app/commu/patient.h"


class TestPatient : public QObject
{
    Q_OBJECT

public:
    TestPatient();
    ~TestPatient();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
    void test_case2();
    void test_case3();
    void test_case4();
    void test_case5();

private:
    communhttp requester;
    userinfo usr;
};

#endif // TEST_PATIENT_H
