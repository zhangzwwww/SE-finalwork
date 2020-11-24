#ifndef COMMUNHTTP_H
#define COMMUNHTTP_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

class communhttp : public QObject{
    Q_OBJECT
private:
    // the network manager handling the communication
    QNetworkAccessManager * manager;

private slots:
    // after reply is finished -- do the following
    void replyfinished(QNetworkReply *reply);

public:
    // constructor
    explicit communhttp(QObject *parent = nullptr);

    // Handle all GET operation, given the http request
    QNetworkReply* http_get(QNetworkRequest request);

    // Handle all POST operation, given the http request
    QNetworkReply* http_post(QNetworkRequest request, QByteArray data);

    // Handle all PUT operation, given the http request
    QNetworkReply* http_put(QNetworkRequest request, QByteArray data);

    // Handle all DELETE operation, given the http request
    QNetworkReply* http_delete(QNetworkRequest request);

    // Handle HEAD operation, give http request
    QNetworkReply* http_head(QNetworkRequest request);
};

#endif // COMMUNHTTP_H
