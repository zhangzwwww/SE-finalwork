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
    // construct the request
//    QNetworkRequest request;
//    QString url = "https://www.baidu.com";
//    request.setUrl(QUrl(url));
//    // send request
//    QNetworkReply* reply = requester.http_get(request);
//    qDebug() << reply->readAll();
//    reply->deleteLater();

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
    QString answer = result;
    qDebug() << answer;
    QVERIFY(answer == "\"token unavailable\"\n");
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
    QString result = reply->readAll();
    qDebug() << result;

}

void TestCommun::test_case3(){
    QString email = "lch@test.com";
    QString pw = "hello123";

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
    QString answer = reply->readAll();
    qDebug() << answer;
}
