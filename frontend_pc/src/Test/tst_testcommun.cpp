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
    QNetworkRequest request;
    QString url = "https://www.baidu.com";
    request.setUrl(QUrl(url));
    // send request
    QNetworkReply* reply = requester.http_get(request);
    qDebug() << reply->readAll();
    reply->deleteLater();
}
