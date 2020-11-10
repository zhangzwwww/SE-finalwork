#include "tst_testcommun.h"

TestCommun::TestCommun(){

}

TestCommun::~TestCommun(){

}

void TestCommun::initTestCase(){

}

void TestCommun::cleanupTestCase(){

}

void TestCommun::test_case1(){
    // check user initialization state
    QVERIFY(usr.is_logged_in() == false);

    // test get info with fake token
    QString url = urlbase["base"] + urlbase["auth"] + "/token";
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QString token = "a96e346250e94201b11fc97b4a1529e4";
    request.setRawHeader("X-Auth-Token", token.toUtf8());
    QNetworkReply* reply = requester.http_get(request);
    QByteArray result = reply->readAll();
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QVERIFY(status == 401);
    reply->deleteLater();
}

void TestCommun::test_case2(){
    QString email = "test@test.com";
    QString pw = "hello123";

    // test get user info from server
    QString url = urlbase["base"] + urlbase["auth"] + "/token";
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    // construct data
    QJsonObject json_content;
    QJsonDocument json_doc;
    json_content.insert("email", email);
    json_content.insert("password", pw);
    json_doc.setObject(json_content);
    QByteArray data = json_doc.toJson(QJsonDocument::Compact);
    // send request
    QNetworkReply* reply = requester.http_post(request, data);
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QString header = reply->rawHeader("X-Auth-Token");
    QVERIFY(status == 200);
    reply->deleteLater();

    // test get fake user info
    QNetworkRequest req3;
    req3.setUrl(QUrl(url));
    req3.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    QJsonObject json_content3;
    QJsonDocument json_doc3;
    json_content3.insert("email", email);
    json_content3.insert("password", "aaaa");
    json_doc3.setObject(json_content3);
    QByteArray data3 = json_doc3.toJson(QJsonDocument::Compact);
    QNetworkReply* rep3 = requester.http_post(req3, data3);
    status = rep3->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QVERIFY(status == 401);
    rep3->deleteLater();

    // then get info with true token
    QNetworkRequest request2;
    request2.setUrl(QUrl(url));
    request2.setRawHeader("X-Auth-Token", header.toUtf8());
    QNetworkReply* reply2 = requester.http_get(request2);
    status = reply2->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    // check reply has no error
    QVERIFY(reply2->error() == false);
    QVERIFY(status == 200);

    QByteArray answer = reply2->readAll();
    // get the json format answer
    QJsonParseError jerror;
    QJsonDocument json = QJsonDocument::fromJson(answer, &jerror);
    // check json parser has no problem
    QVERIFY(jerror.error == QJsonParseError::NoError);
    QVERIFY(json.isObject());
    // check user info align with what we got
    QVERIFY(json.object()["email"].toString() == email);
    reply2->deleteLater();
}

void TestCommun::test_case3(){
    // test the head token
    QString email = "test@test.com";
    QString pw = "hello123";
    QString url = urlbase["base"] + urlbase["auth"] + "/token";
    // first get the token
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    // construct data
    QJsonObject json_content;
    QJsonDocument json_doc;
    json_content.insert("email", email);
    json_content.insert("password", pw);
    json_doc.setObject(json_content);
    QByteArray data = json_doc.toJson(QJsonDocument::Compact);
    // send request
    QNetworkReply* reply = requester.http_post(request, data);
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QString token = reply->rawHeader("X-Auth-Token");
    QVERIFY(status == 200);
    reply->deleteLater();

    // send head request -- validate token
    QNetworkRequest request2;
    request2.setUrl(QUrl(url));
    request2.setRawHeader("X-Auth-Token", token.toUtf8());
    QNetworkReply* reply2 = requester.http_head(request2);
    status = reply2->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QVERIFY(status == 200);

    // delete the test user
    QNetworkRequest req3;
    req3.setUrl(QUrl(url));
    req3.setRawHeader("X-Auth-Token", token.toUtf8());
    QNetworkReply* rep3 = requester.http_delete(req3);
    status = rep3->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QVERIFY(status == 204);
}

void TestCommun::test_case4(){
    // Test register user into system

    QString email = "test@test.com";
    QString pw = "hello123";

    // first delete

    // test register a user into server
    QString url = urlbase["base"] + urlbase["auth"] + "/register";
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    // construct data
    QJsonObject json_content;
    QJsonDocument json_doc;
    json_content.insert("email", email);
    json_content.insert("password", pw);
    json_doc.setObject(json_content);
    QByteArray data = json_doc.toJson(QJsonDocument::Compact);
    // send request
    QNetworkReply* reply = requester.http_post(request, data);
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QVERIFY(status == 400);
    // QString token = reply->rawHeader("X-Auth-Token");
}

void TestCommun::test_case5(){
    // test the userinfo class
    // test register
    QString email = "test@test.com";
    QString password = "hello123";
//    bool result = usr.register_user_info(&requester, email, password);
//    QVERIFY(result == true);

    // test log in
    bool result = usr.log_user_in(&requester, email, password);
    QVERIFY(result == true);
    QVERIFY(usr.is_logged_in() == true);
    QVERIFY(usr._email() == email);

    // get the info of user and check
    bool getInfo = usr.get_user_info(&requester);
    QVERIFY(getInfo == true);
    QVERIFY(usr._email() == email);

    // head the token
    bool valiTok = usr.head_token(&requester);
    QVERIFY(valiTok == true);

    // delete the token
    bool delTok = usr.delete_user_token(&requester);
    QVERIFY(delTok == true);
    QVERIFY(usr._token() == "");

    // get a new token
    bool getTok = usr.get_user_token(&requester, email, password);
    QVERIFY(getTok == true);
    QVERIFY(usr._token() != "");

}
