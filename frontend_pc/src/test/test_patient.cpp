#include "test_patient.h"

TestPatient::TestPatient()
{

}

TestPatient::~TestPatient(){

}

void TestPatient::initTestCase(){
    // first log in the user
    QString email = "test@test.com";
    QString pwd = "hello123";
    QVERIFY(usr.log_user_in(&requester, email, pwd));
    QVERIFY(usr._token() != "");
    // set the token of the patient class
    patient::set_token(usr._token());
}

void TestPatient::cleanupTestCase(){

}

void TestPatient::test_case1(){
    // verify the user information
    QVERIFY(usr._email() == "test@test.com");
}

void TestPatient::test_case2(){

}

void TestPatient::test_case3(){

}

void TestPatient::test_case4(){

}

void TestPatient::test_case5(){

}
