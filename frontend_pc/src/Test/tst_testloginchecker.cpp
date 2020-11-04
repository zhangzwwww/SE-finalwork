#include <QtTest>
#include <QCoreApplication>

#include "../App/Login/loginchecker.h"

// add necessary includes here

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

TestLoginChecker::TestLoginChecker()
{

}

TestLoginChecker::~TestLoginChecker()
{

}

void TestLoginChecker::initTestCase()
{

}

void TestLoginChecker::cleanupTestCase()
{

}

void TestLoginChecker::test_case1()
{
    QVERIFY(m_login_checker.SignIn("1", "1") == OK);
}

// QTEST_MAIN(TestLoginChecker)

// #include "tst_testloginchecker.moc"
