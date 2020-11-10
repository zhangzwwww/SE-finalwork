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

    id = "";
    email = "";
    ctime = "";
    role = "";
    password = "";

    token = "";
}

QString userinfo::_email(){
    return email;
}

QString userinfo::_token(){
    return token;
}

bool userinfo::is_logged_in(){
    return is_login;
}

void userinfo::update_login(bool status){
    is_login = status;
}

bool userinfo::log_user_in(communhttp *requester, QString email, QString pw){
    // Send request to server to verify
    if (get_user_token(requester, email, pw)){
        // successfully get token
        // store the email and pw infos
        this->email = email;
        this->password = pw;
        update_login(true);
        return true;
    }

    // The case response fails
    return false;
}

bool userinfo::get_user_info(communhttp* requester){
    QString url = urlbase["base"] + urlbase["auth"] + "/token";
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Auth-Token", this->token.toUtf8());
    // send the request
    QNetworkReply* reply = requester->http_get(request);
    // handle the reply given by the server
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status == 401){
        // unaothorized
        qDebug() << "token unauthorized";
        reply->deleteLater();
        return false;
    }
    if (status == 200){
        QByteArray resp = reply->readAll();
        QJsonParseError jerror;
        QJsonDocument json = QJsonDocument::fromJson(resp, &jerror);
        if (jerror.error == QJsonParseError::NoError && json.isObject()){
            this->email = json.object()["email"].toString();
            this->ctime = json.object()["ctime"].toString();
            this->id = json.object()["id"].toString();
            this->role = json.object()["role"].toString();
        }
        else {
            qDebug() << "Response json format error";
            reply->deleteLater();
            return false;
        }
    }
    // free the reply object
    reply->deleteLater();
    return true;
}

bool userinfo::register_user_info(communhttp *requester, QString email, QString pw){
    QString url = urlbase["base"] + urlbase["auth"] + "/register";
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

    // construct the data needed to post
    QJsonObject json_content;
    QJsonDocument json_doc;

    json_content.insert("email", email);
    json_content.insert("password", pw);
    json_doc.setObject(json_content);
    QByteArray data = json_doc.toJson(QJsonDocument::Compact);

    // send the request
    QNetworkReply* reply = requester->http_post(request, data);
    // Handle the reply sent by server
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    // Handle error case
    if (status == 404){
        qDebug() << "404 Not Found";
        reply->deleteLater();
        return false;
    }
    if (status == 400 || status == 409){
        qDebug() << "Bad Request";
        reply->deleteLater();
        return false;
    }
    // Handle the normal case
    if (status == 201){
        // store the information
        this->token = reply->rawHeader("X-Auth-Token");
        this->email = email;
        this->password = pw;
        update_login(true);
    }

    // free the reply object
    reply->deleteLater();
    return true;
}

bool userinfo::get_user_token(communhttp *requester, QString email, QString pw){
    QString url = urlbase["base"] + urlbase["auth"] + "/token";
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

    // construct the data needed to post
    QJsonObject json_content;
    QJsonDocument json_doc;

    json_content.insert("email", email);
    json_content.insert("password", pw);
    json_doc.setObject(json_content);
    QByteArray data = json_doc.toJson(QJsonDocument::Compact);

    // send the request
    QNetworkReply* reply = requester->http_post(request, data);
    // Handle the reply sent by server
    // Handle error case first
    if (reply->error()){
        // request fails
        qDebug() << reply->errorString();
        reply->deleteLater();
        return false;
    }
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status == 401){
        // bad request
        qDebug() << "Email or password unauthorized";
        reply->deleteLater();
        return false;
    }
    if (status == 200){
        // status ok
        // store the token for future use
        this->token = reply->rawHeader("X-Auth-Token");
    }
    // free the reply object
    reply->deleteLater();
    return true;
}

bool userinfo::delete_user_token(communhttp *requester){
    QString url = urlbase["base"] + urlbase["auth"] + "/token";
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Auth-Token", this->token.toUtf8());

    // send the request
    QNetworkReply* reply = requester->http_delete(request);
    // Handle the reply given by the server
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    // Handle error cases
    if (status == 405){
        qDebug() << "405 Not Allowed";
        reply->deleteLater();
        return false;
    }
    if (status == 401){
        qDebug() << "Token unauthorized";
        reply->deleteLater();
        return false;
    }
    // Handle normal case
    if (status == 204){
        // clear the local token
        this->token = "";
    }
    // free the reply object
    reply->deleteLater();
    return true;
}

bool userinfo::head_token(communhttp *requester){
    QString url = urlbase["base"] + urlbase["auth"] + "/token";
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Auth-Token", this->token.toUtf8());

    // send the request
    QNetworkReply* reply = requester->http_head(request);
    // Handle the reply given by the server
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    // Handle error cases
    if (status == 404){
        qDebug() << "404 Not found";
        reply->deleteLater();
        return false;
    }
    if (status == 401){
        qDebug() << "Token unauthorized";
        reply->deleteLater();
        return false;
    }
    // Handle normal case
    if (status == 200){
        // do nothing -- just return true
    }
    // free the reply object
    reply->deleteLater();
    return true;
}
