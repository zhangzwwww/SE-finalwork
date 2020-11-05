#ifndef TST_TESTLOGINCHECKER_H
#define TST_TESTLOGINCHECKER_H

#endif // TST_TESTLOGINCHECKER_H

#include <QtTest>
#include <QCoreApplication>

#include "../app/login/loginchecker.h"

class TestLoginChecker : public QObject
{
    Q_OBJECT

public:
    TestLoginChecker();
    ~TestLoginChecker();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

private:
    LoginChecker m_login_checker;
};
