#include "communhttp.h"

/* Communication with http class.
 * The method provide an interface for basic operations of http
 *
 * Note: in the whole application, only need keep
 * one communhttp instance
 */

communhttp::communhttp(QObject *parent) : QObject (parent)
{
    manager = new QNetworkAccessManager();
    QMetaObject::Connection connn = connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyfinished(QNetworkReply*)));
    //    Debug part
//    QNetworkRequest request;
//    request.setUrl(QUrl("https://www.baidu.com"));
//    QNetworkReply* reply = manager->get(request);
//    QEventLoop eventloop;
//    connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
//    eventloop.exec();
//    if (reply->error() == QNetworkReply::NoError)
//        qDebug() << "request no error";
//    else{
//        qDebug() << "request error";
//    }
//    QByteArray result = reply->readAll();
//    qDebug() << result;

}

void communhttp::replyfinished(QNetworkReply *reply){
    // this slot function for debugging if error happens
    // qDebug("reply finished");
    if (reply->error()){
        qDebug() << reply->errorString();
        return;
    }
}

QNetworkReply* communhttp::http_get(QNetworkRequest request){
    QNetworkReply* reply = manager->get(request);
    // loop until get request
    QEventLoop eventloop;
    connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
    eventloop.exec();
    // request got
    return reply;
}

QNetworkReply* communhttp::http_post(QNetworkRequest request, QByteArray data){
    QNetworkReply* reply = manager->post(request, data);
    // loop until get request
    QEventLoop eventloop;
    connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
    eventloop.exec();
    // request got
    return reply;
}

QNetworkReply* communhttp::http_put(QNetworkRequest request, QByteArray data){
    QNetworkReply* reply = manager->put(request, data);
    // loop until put request
    QEventLoop eventloop;
    connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
    eventloop.exec();
    // request got
    return reply;
}

QNetworkReply* communhttp::http_head(QNetworkRequest request){
    QNetworkReply* reply = manager->head(request);
    // loop until get request
    QEventLoop eventloop;
    connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
    eventloop.exec();
    // request got
    return reply;
}

QNetworkReply* communhttp::http_delete(QNetworkRequest request){
    QNetworkReply* reply = manager->deleteResource(request);
    // loop until get request
    QEventLoop eventloop;
    connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
    eventloop.exec();
    // request got
    return reply;
}
