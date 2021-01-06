#include "prediction.h"

QString prediction::token = "";

prediction::prediction()
{

}

void prediction::set_token(QString t){
    token = t;
}

QString prediction::predictTumor(communhttp* requester, QString filepath){
    QFile afile(filepath);
    if (!afile.exists()){
        qDebug() << "The file doesn't exist!";
        return "";
    }
    // Read in the file
    afile.open(QIODevice::ReadOnly);
    int file_len = afile.size();
    QDataStream in(&afile);
    char* buf = new char[file_len];
    in.readRawData(buf, file_len);
    afile.close();

    // construct the request
    QUrl url(urlbase["base4"] + urlbase["predict"]);
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("X-Auth-Token", token.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("multipart/form-data"));
    // construct the data
    QByteArray post_data;
    QByteArray data = QByteArray(buf, file_len);
    post_data.append("tumor_image=" + data);

    // send request
    QNetworkReply* reply = requester->http_post(request, post_data);
    // handle response
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status != 200){
        qDebug() << "Some error happens";
        return "";
    }
    QByteArray resp = reply->readAll();
    QString result = resp;
    return result;
}
