#include "tst_testloginchecker.h"

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
