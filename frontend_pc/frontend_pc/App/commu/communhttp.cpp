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
    connect(manager, &QNetworkAccessManager::finished,
            this, &communhttp::replyfinished);
}

void communhttp::replyfinished(QNetworkReply *reply){
    if (reply->error()){
        qDebug() << reply->errorString();
        return;
    }
    QString answer = reply->readAll();
    qDebug() << answer;
}

QNetworkReply* communhttp::http_get(QNetworkRequest request){
    return manager->get(request);
}

QNetworkReply* communhttp::http_post(QNetworkRequest request, QByteArray data){
    return manager->post(request, data);
}

QNetworkReply* communhttp::http_head(QNetworkRequest request){
    return manager->head(request);
}

QNetworkReply* communhttp::http_delete(QNetworkRequest request){
    return manager->deleteResource(request);
}
