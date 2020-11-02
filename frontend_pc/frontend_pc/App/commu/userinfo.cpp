#include "userinfo.h"

/* The userinfo contains information of the user.
 * The class provide the method that can communicate with server.
 * Note:
 * - Before using the communication methods, you need to declare
 *   a communhttp instance first.
 * - At the beginning of the application, there should declare a
 *   userinfo object as the current user.
 * - There should only exist one userinfo object during lifetime
 *   of application.
 */
userinfo::userinfo()
{
    is_login = false;

    id = -1;
    email = "";
    ctime = "";
    role = "";
    password = "";

    token = "";
}

bool userinfo::is_logged_in(){
    return is_login;
}

void userinfo::update_login(bool status){
    is_login = status;
}

bool userinfo::get_user_info(communhttp* requester){
    QString url = urlbase["auth"] + "/tokens";
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Auth-Token", this->token.toUtf8());
    // send the request
    QNetworkReply* reply = requester->http_get(request);
    // TODO: handle the reply given by the server


    // free the reply object
    reply->deleteLater();
    return true;
}

bool userinfo::register_user_info(communhttp *requester){
    QString url = urlbase["auth"] + "/register";
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

    // construct the data needed to post
    QJsonObject json_content;
    QJsonDocument json_doc;

    /* these info handled by server
    json_content.insert("id", this->id);
    json_content.insert("ctime", this->ctime);
    json_content.insert("role", this->role);
    */

    json_content.insert("email", this->email);
    json_content.insert("password", this->password);
    json_doc.setObject(json_content);
    QByteArray data = json_doc.toJson(QJsonDocument::Compact);

    // send the request
    QNetworkReply* reply = requester->http_post(request, data);
    // TODO: handle the reply sent by server


    // free the reply object
    reply->deleteLater();
    return true;
}

bool userinfo::get_user_token(communhttp *requester){
    QString url = urlbase["auth"] + "/tokens";
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

    // construct the data needed to post
    QJsonObject json_content;
    QJsonDocument json_doc;
    json_content.insert("id", this->id);
    json_content.insert("email", this->email);
    json_content.insert("ctime", this->ctime);
    json_content.insert("role", this->role);
    json_content.insert("password", this->password);
    json_doc.setObject(json_content);
    QByteArray data = json_doc.toJson(QJsonDocument::Compact);

    // send the request
    QNetworkReply* reply = requester->http_post(request, data);
    // TODO: handle the reply sent by server


    // free the reply object
    reply->deleteLater();
    return true;
}

bool userinfo::delete_user_token(communhttp *requester){
    QString url = urlbase["auth"] + "/tokens";
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Auth-Token", this->token.toUtf8());

    // send the request
    QNetworkReply* reply = requester->http_delete(request);
    // handle the reply given by the server


    // free the reply object
    reply->deleteLater();
    return true;
}

bool userinfo::head_token(communhttp *requester){
    QString url = urlbase["auth"] + "/tokens";
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Auth-Token", this->token.toUtf8());

    // send the request
    QNetworkReply* reply = requester->http_head(request);
    // handle the reply given by the server


    // free the reply object
    reply->deleteLater();
    return true;
}
