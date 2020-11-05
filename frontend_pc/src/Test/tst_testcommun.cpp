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

void TestCommun::test_case4(){

}