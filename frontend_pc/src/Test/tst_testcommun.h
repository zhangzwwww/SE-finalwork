#ifndef TST_TESTCOMMUN_H
#define TST_TESTCOMMUN_H

#endif // TST_TESTCOMMUN_H

#include <QTest>
#include <QCoreApplication>
#include <QNetworkAccessManager>

#include "../App/commu/communhttp.h"
#include "../App/commu/userinfo.h"

class TestCommun : public QObject
{
    Q_OBJECT

public:
    TestCommun();
    ~TestCommun();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
    void test_case2();
    void test_case3();

private:
    communhttp requester;
    userinfo usr;
};
