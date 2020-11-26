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
    // test get all patient info
    QVector<patient> result = patient::http_get_all_patient(&requester);
    int size1 = result.size();
    // test create new patient
    bool resp = patient::http_create_patient(&requester, "person1", true, "2020-11-26", 0);
    result = patient::http_get_all_patient(&requester);
    int size2 = result.size();
    if (resp){
        // creation succeeds
        qDebug() << "create succeeds. size: " << size2;
        QVERIFY(size1 + 1 == size2);
    }
    else {
        // creation fails
        qDebug() << "create falis. size: " << size1;
        QVERIFY(size1 == size2);
    }
}

void TestPatient::test_case2(){
    // test get info by id
    QVector<patient> result = patient::http_get_all_patient(&requester);
    patient person1 = result[result.size() - 1];
    QString id = person1._id();
    // get info of a certain patient
    patient resp = patient::http_getPatient_byId(&requester, id);
    QVERIFY(resp._id() == person1._id());
    QVERIFY(resp._name() == person1._name());
    QVERIFY(resp._gender() == person1._gender());
    QVERIFY(resp._birth() == person1._birth());
    QVERIFY(resp._age() == person1._age());
}

void TestPatient::test_case3(){
    // test modify info by id
    QVector<patient> result = patient::http_get_all_patient(&requester);
    patient person1 = result[0];
    QString id = person1._id();
    // modify the information
    bool resp = patient::http_modifyPatient_byId(&requester, id, "someperson", false, "2000-01-01", 20);
    QVERIFY(resp);
}

void TestPatient::test_case4(){
    // test delete certain patient info
    QVector<patient> result1 = patient::http_get_all_patient(&requester);
    patient person1 = result1[0];
    QString id = person1._id();
    // delete the info
    bool resp = patient::http_deletePatient_byId(&requester, id);
    QVERIFY(resp);
    QVector<patient> result2 = patient::http_get_all_patient(&requester);
    QVERIFY(result1.size() == result2.size() + 1);
}

void TestPatient::test_case5(){

}
