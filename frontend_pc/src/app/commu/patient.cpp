#include "patient.h"


// initialize the static member
QString patient::token = "";


// empty constructor
patient::patient()
{
    id = "";
    name = "";
    gender = false;
    birth = "";
    age = -1;
}


// construct with information given
patient::patient(QString n, bool g, QString b, int a, QString i){
    name = n;
    gender = g;
    birth = b;
    age = a;
    id = i;
}

// set patient info manually by user
void patient::set_patient_info(QString name, bool gender, QString birth, int age){
    this->name = name;
    this->gender = gender;
    this->birth = birth;
    this->age = age;
}

// get patient information
QString patient::_id(){
    return this->id;
}
QString patient::_name(){
    return this->name;
}
bool patient::_gender(){
    return this->gender;
}
QString patient::_birth(){
    return this->birth;
}
int patient::_age(){
    return this->age;
}

// set the communication token
void patient::set_token(QString t){
    token = t;
}

// get all patient info on the server
// return a list of patients if success
QVector<patient> patient::http_get_all_patient(communhttp *req){
    QString url = urlbase["base2"] + urlbase["patient"];
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Auth-Token", token.toUtf8());
    // send request
    QNetworkReply* reply = req->http_get(request);
    // handle the reply
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QVector<patient> result;
    if (status == 200){
        // normal case
        QByteArray resp = reply->readAll();
        QJsonParseError jerror;
        QJsonDocument json = QJsonDocument::fromJson(resp, &jerror);
        if (jerror.error == QJsonParseError::NoError && !json.isNull() && !json.isEmpty()){
            // parse each patient
            QVariantList list = json.toVariant().toList();
            for (int i = 0; i < list.count(); i++){
                QVariantMap map = list[i].toMap();
                result.push_back(patient(
                        map["name"].toString(),
                        map["gender"].toBool(),
                        map["birth"].toString(),
                        map["age"].toInt(),
                        map["id"].toString())
                        );
            }
        }
        else {
            qDebug() << "Json parsing error";
            reply->deleteLater();
            return result;
        }
        return result;
    }
    else if (status == 401){
        // the token is unauthorized
        qDebug() << "token unauthorized";
        reply->deleteLater();
        return result;
    }
    else {
        // other error
        qDebug() << "http error";
        reply->deleteLater();
        return result;
    }
}

// post a new patient
// return true if success
bool patient::http_create_patient(communhttp *req, QString name, bool gender, QString birth, int age){
    QString url = urlbase["base2"] + urlbase["patient"];
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader("X-Auth-Token", token.toUtf8());
    // construct body data
    QJsonObject json_content;
    QJsonDocument json_doc;
    json_content.insert("name", name);
    json_content.insert("gender", gender);
    json_content.insert("birth", birth);
    json_content.insert("age", age);
    json_doc.setObject(json_content);
    QByteArray data = json_doc.toJson(QJsonDocument::Compact);

    // send the request
    QNetworkReply* reply = req->http_post(request, data);
    // Handle reply sent by server
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status == 400){
        qDebug() << "Bad request";
        reply->deleteLater();
        return false;
    }
    if (status == 401){
        qDebug() << "Token unauthorized";
        reply->deleteLater();
        return false;
    }
    // handle normal case
    if (status == 201){
        reply->deleteLater();
        return true;
    }
    // other error
    reply->deleteLater();
    return false;
}

// get a patient by id
// return a patient object if success
patient patient::http_getPatient_byId(communhttp *req, QString id){
    QString url = urlbase["base2"] + urlbase["patient"] + '/' + id;
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Auth-Token", token.toUtf8());
    // send request
    QNetworkReply* reply = req->http_get(request);
    // handle the reply
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status == 401){
        // unauthorized
        qDebug() << "token unauthorized";
        reply->deleteLater();
        return patient();
    }
    else if (status == 400){
        // bad request
        qDebug() << "Bad request";
        reply->deleteLater();
        return patient();
    }
    else if (status == 200){
        // normal case
        QByteArray resp = reply->readAll();
        QJsonParseError jerror;
        QJsonDocument json = QJsonDocument::fromJson(resp, &jerror);
        if (jerror.error == QJsonParseError::NoError && json.isObject()){
            QString i = json.object()["id"].toString();
            patient result(
                    json.object()["name"].toString(),
                    json.object()["gender"].toVariant().toBool(),
                    json.object()["birth"].toString(),
                    json.object()["age"].toInt(),
                    i
                    );
            reply->deleteLater();
            return result;
        }
        else {
            qDebug() << "Response json format error";
            reply->deleteLater();
            return patient();
        }
    }
    else {
        qDebug() << "some error happens";
        reply->deleteLater();
        return patient();
    }
}

// modify a patient by id
// return true if success
bool patient::http_modifyPatient_byId(communhttp *req, QString id, QString name, bool gender, QString birth, int age){
    QString url = urlbase["base2"] + urlbase["patient"] + '/' + id;
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader("X-Auth-Token", token.toUtf8());
    // construct body data
    QJsonObject json_content;
    QJsonDocument json_doc;
    json_content.insert("name", name);
    json_content.insert("gender", gender);
    json_content.insert("birth", birth);
    json_content.insert("age", age);
    json_content.insert("id", id);
    json_doc.setObject(json_content);
    QByteArray data = json_doc.toJson(QJsonDocument::Compact);

    // send request
    QNetworkReply* reply = req->http_put(request, data);
    // Handle reply sent by server
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status == 400){
        qDebug() << "Bad request";
        reply->deleteLater();
        return false;
    }
    if (status == 401){
        qDebug() << "Token unauthorized";
        reply->deleteLater();
        return false;
    }
    if (status == 405){
        qDebug() << "Undocumented";
        reply->deleteLater();
        return false;
    }
    if (status == 204){
        // handle normal case
        reply->deleteLater();
        return true;
    }
    // other error
    reply->deleteLater();
    return false;
}

// delete a patient by id
// return true if success
bool patient::http_deletePatient_byId(communhttp *req, QString id){
    QString url = urlbase["base2"] + urlbase["patient"] + '/' + id;
    // construct the request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Auth-Token", token.toUtf8());

    // send the request
    QNetworkReply* reply = req->http_delete(request);
    // Handle the reply given by the server
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    // Handle error cases
    if (status == 405){
        qDebug() << "Not allowed";
        reply->deleteLater();
        return false;
    }
    if (status == 401){
        qDebug() << "Token unauthorized";
        reply->deleteLater();
        return false;
    }
    if (status == 400){
        qDebug() << "Bad request";
        reply->deleteLater();
        return false;
    }
    // Handle normal case
    if (status == 204){
        // delete success
        reply->deleteLater();
        return true;
    }
    // other error
    reply->deleteLater();
    return false;
}
