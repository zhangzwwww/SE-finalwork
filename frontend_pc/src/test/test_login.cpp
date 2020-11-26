#include "test_login.h"

TestLogin::TestLogin()
{

}

TestLogin::~TestLogin()
{

}

void TestLogin::initTestCase()
{

}

void TestLogin::cleanupTestCase()
{

}

void TestLogin::test_case1()
{
    communhttp commun;
    userinfo user;
    QVERIFY(m_login_checker.SignIn("test@test.com", "hello123", &commun, &user) == OK);
    QVERIFY(user.is_logged_in() == true);
    QVERIFY(user._email() == "test@test.com");
}
