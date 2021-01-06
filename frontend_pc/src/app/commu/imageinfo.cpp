#include "imageinfo.h"
#include "../app/errorcode.h"


// construct a progress window to show downloading progress
ProgressDialog::ProgressDialog(const QUrl &url, QWidget *parent, QString progress) : QProgressDialog (parent){
    setWindowTitle(tr(progress.toUtf8() + " Progress"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setLabelText(tr(progress.toUtf8() + "ing %1.").arg(url.toDisplayString()));
    setMinimum(0);
    setValue(0);
    setMinimumDuration(0);
    setMinimumSize(QSize(400, 75));
    this->setCancelButton(nullptr);
}

ProgressDialog::~ProgressDialog(){

}

void ProgressDialog::networkReplyProgress(qint64 bytesRead, qint64 totalBytes){
    setMaximum(totalBytes);
    setValue(bytesRead);
}


QString imageInfo::token = "";
QString imageInfo::save_path = "";

imageInfo::imageInfo(QObject* parent)
{
    connect(this, &imageInfo::sendreq, this, &imageInfo::uploadFile);
}

imageInfo::~imageInfo(){

}

void imageInfo::setFilePath(QString path){
    save_path = path;
}

void imageInfo::setToken(QString t){
    token = t;
}

void imageInfo::getImagesHttp(QString patientID, QString ctime){
    // construct a directory to save files
    // TODO: check whether they contain files
    QDir dir(save_path);
    if (!dir.exists()){
        qDebug() << "invalid saving path";
        return;
    }
    dir.mkdir(patientID + "_" + ctime);
    if (!dir.cd(patientID + "_" + ctime)){
        qDebug() << "create directory fails";
        return;
    }
    folder_path = dir.absolutePath();
    // write in the metadata
    QString fullpath = dir.filePath("meta_data");
    QFile afile(fullpath);
    if (!afile.open(QIODevice::WriteOnly)){
        qDebug() << "Open file fails";
        return;
    }
    afile.write(patientID.toUtf8(), patientID.length());
    afile.write("\r\n");
    afile.write(ctime.toUtf8(), ctime.length());
    afile.write("\r\n");

    // construct url and query item
    QUrl url(urlbase["base2"] + urlbase["image"]);
    QUrlQuery query;
    query.addQueryItem("patientId", patientID);
    query.addQueryItem("ctime", ctime);
    url.setQuery(query.query());
    // construct request
    // Asynchronized way
    connect(&qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFile(QNetworkReply*)));

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("X-Auth-Token", token.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    // send request
    QNetworkReply* reply = qnam.get(request);

    // construct the dialog window
    ProgressDialog *progressDialog = new ProgressDialog(url, nullptr);
    progressDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(progressDialog, &QProgressDialog::canceled, this, &imageInfo::cancelDownload);
    connect(reply, &QNetworkReply::downloadProgress, progressDialog, &ProgressDialog::networkReplyProgress);
    connect(reply, &QNetworkReply::finished, progressDialog, &ProgressDialog::hide);
    progressDialog->show();

    // Debug: use straight way to handle reply
//    QEventLoop eventloop;
//    connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
//    eventloop.exec();
//    downloadFile(reply);
}

void imageInfo::downloadFile(QNetworkReply* reply){
    // handle the error
    if (reply->error()){
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }
    // get the status of reply
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status == 401){
        // unauthorized
        qDebug() << "Token unauthorized";
        reply->deleteLater();
        return;
    }
    if (status == 200){
        QDir dir(folder_path);
        // add info to meta file
        QFile meta(dir.filePath("meta_data"));
        if (!meta.open(QIODevice::Append)){
            qDebug() << "Open meta file fails";
            reply->deleteLater();
            return;
        }
        // normal case
        QByteArray resp = reply->readAll();
        QJsonParseError jerror;
        QJsonDocument json = QJsonDocument::fromJson(resp, &jerror);
        if (jerror.error == QJsonParseError::NoError && !json.isNull() && !json.isEmpty()){
            // parse each patient
            QVariantList list = json.toVariant().toList();
            for (int i = 0; i < list.count(); i++){
                // get each data and save file
                QString filename = QString("%1").arg(i, 6, 10, QLatin1Char('0')) + ".dcm";
                QVariantMap map = list[i].toMap();
                // save image
                QString filepath = dir.filePath(filename);
                QFile afile(filepath);
                if (!afile.open(QIODevice::WriteOnly)){
                    qDebug() << "Open file fails";
                    return;
                }
                // TODO: check how to write the file
                QString content = map["content"].toString();
                QString img_id = map["id"].toString();
//                qDebug() << "NEW FIG";
//                qDebug() << content.toLatin1();
                afile.write(content.toLatin1(), content.length());
                meta.write(img_id.toUtf8(), img_id.length());
                meta.write("\r\n");
            }
        }
        meta.close();
    }
    reply->deleteLater();
}

void imageInfo::uploadFolerImage(QString patientId, QString ctime, QString folderpath){
    QDir dir(folderpath);
    // check if there exists meta file
    QFile meta(dir.filePath("meta_data"));
    if (meta.open(QIODevice::ReadOnly)){
        qDebug() << "Already in backend, will not upload!";
        QMessageBox::warning(nullptr, "w", "The folder already in backend, will not upload", QMessageBox::Yes);
        return;
    }
    // upload images
    QStringList file_names = dir.entryList(QDir::Files | QDir::Readable);
    for (QString &file_name : file_names){
        this->uploadImageHttp(patientId, ctime, dir.absoluteFilePath(file_name));
    }
}


void imageInfo::uploadImageHttp(QString patientId, QString ctime, QString filepath){
    QFile afile(filepath);
    if (!afile.exists()){
        qDebug() << "The file doesn't exist";
        return;
    }
    QString URL = urlbase["base2"] + urlbase["image"];
    QString req = "curl --location --request POST \"" + URL + "\"";
    req = req + " --header \"X-Auth-Token: " + token.toUtf8() + "\"";
    req = req + " --form \"uploaded_file=@" + filepath + "\"";
    req = req + " --form \"patientId=" + patientId + "\"";
    req = req + " --form \"filename=" + ctime + "\"";

    emit sendreq(req);

//#ifdef Q_OS_WIN32   // Define on windows system
//    fp = _popen(req.toUtf8(), "r");
//#endif

//#ifdef Q_OS_MACOS   // Define on MACOS system

//    fp = popen(req.toUtf8(), "r");
//#endif
//    QHttpMultiPart* multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
//    QHttpPart imagePart;
//    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
//    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"uploaded_file\""));
//    QFile *imgfile = new QFile(filepath);
//    // QFile imgfile(filepath);
//    imgfile->open(QIODevice::ReadOnly);
//    imagePart.setBodyDevice(imgfile);
//    imgfile->setParent(multipart);
//    multipart->append(imagePart);

//    QHttpPart textpart;
//    textpart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"patientId\""));
//    textpart.setBody(patientId.toUtf8());

//    QHttpPart textpart2;
//    textpart2.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"ctime\""));
//    textpart2.setBody(ctime.toUtf8());

//    multipart->append(textpart);
//    multipart->append(textpart2);

//    QUrl url_test(urlbase["base2"] + urlbase["image"]);
//    QNetworkRequest request_test;
//    request_test.setRawHeader("X-Auth-Token", token.toUtf8());
//    request_test.setUrl(url_test);
//    QNetworkReply* reply_test = qnam.post(request_test, multipart);
//    qDebug() << "first request";
//        // Debug: use straight way to handle reply
//    QEventLoop eventloop;
//    connect(reply_test, SIGNAL(finished()), &eventloop, SLOT(quit()));
//    eventloop.exec();
//    httpFinished(reply_test);

    // Read in the file
//    afile.open(QIODevice::ReadOnly);
//    QByteArray data = afile.readAll();
//    afile.close();

//    // construct the request
//    QUrl url(urlbase["base3"] + urlbase["image"]);
//    // QUrl url("http://10.166.95.178:5001/v1/predict");
//    QNetworkRequest request;
//    request.setUrl(url);
//    request.setRawHeader("X-Auth-Token", token.toUtf8());
//    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("multipart/form-data"));
//    // construct the data
//    QByteArray post_data;
//    post_data.append("patientId=" + patientId);
//    post_data.append("filename =" + ctime);
//    // qDebug() << data;
//    post_data.append("uploaded_file=" + data.toBase64());

//    //connect(&qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpFinished(QNetworkReply*)));

//    QNetworkReply* reply = qnam.post(request, post_data);

//    // construct the dialog window
//    ProgressDialog *progressDialog = new ProgressDialog(url, nullptr, "Upload");
//    progressDialog->setAttribute(Qt::WA_DeleteOnClose);
//    connect(progressDialog, &QProgressDialog::canceled, this, &imageInfo::cancelDownload);
//    connect(reply, &QNetworkReply::uploadProgress, progressDialog, &ProgressDialog::networkReplyProgress);
//    connect(reply, &QNetworkReply::finished, progressDialog, &ProgressDialog::hide);
//    progressDialog->show();

//    // Debug: use straight way to handle reply
//    QEventLoop eventloop;
//    connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
//    eventloop.exec();
//    httpFinished(reply);
}

void imageInfo::uploadFile(QString req){
    qDebug() << req;
    FILE *fp;
#ifdef Q_OS_WIN32   // Define on windows system
    fp = _popen(req.toUtf8(), "r");
#endif

#ifdef Q_OS_MACOS   // Define on MACOS system

    fp = popen(req.toUtf8(), "r");
#endif
//    pclose(fp);
}

int imageInfo::uploadImgMark(QString folderpath, int level, int view, double topX, double topY, double bottomX, double bottomY){
    QDir dir(folderpath);
    QFile meta(dir.filePath("meta_data"));
    if (!meta.open(QIODevice::ReadOnly)){
        qDebug() << "The meta file doesn't exist";
        return INVALID_FILEPATH;
    }
    QString content;
    // pass the first two lines
    content = meta.readLine();
    content = meta.readLine();
    // read in image ids
    QVector<QString> img_ids;
    while(!meta.atEnd()){
        content = meta.readLine();
        content.remove("\r\n");
        img_ids.push_back(content);
    }
    if (level < 0 || level >= img_ids.size()){
        qDebug() << "image level out of range!";
        return INVALID_LAYER;
    }
    if (view < 0 || view >= 3){
        qDebug() << "invalid view type!";
        return INVALID_VIEW;
    }
    QString img_id = img_ids[level];
    // construct body data
    QJsonObject json_content;
    QJsonDocument json_doc;
    json_content.insert("layer", level);
    json_content.insert("view", view);
    json_content.insert("topX", topX);
    json_content.insert("topY", topY);
    json_content.insert("bottomX", bottomX);
    json_content.insert("bottomY", bottomY);
    json_content.insert("imageId", img_id);
    json_doc.setObject(json_content);
    QByteArray data = json_doc.toJson(QJsonDocument::Compact);
    // construct request
    QNetworkAccessManager local_qnam;
    QNetworkRequest request;
    QUrl url(urlbase["base2"] + urlbase["mark"]);
    request.setUrl(url);
    request.setRawHeader("X-Auth-Token", token.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    // send request
    QNetworkReply* reply = local_qnam.post(request, data);
    // wait until server reply
    QEventLoop eventloop;
    connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
    eventloop.exec();

    // handle the reply
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status == 201){
        // normal case
        reply->deleteLater();
        return SUCCESS;
    }
    else {
        // bad request
        qDebug() << "Request error: " << status << reply->readAll();
        reply->deleteLater();
        return REQUEST_FAIL;
    }
}

QVector<imageInfo::imgMark> imageInfo::getAllMarks(QString folderpath, int layer){
    // get the imageId first
    QDir dir(folderpath);
    QFile meta(dir.filePath("meta_data"));
    if (!meta.open(QIODevice::ReadOnly)){
        qDebug() << "The meta file doesn't exist!";
        return QVector<imageInfo::imgMark>();
    }
    QString content;
    // pass the first two lines
    content = meta.readLine(); // patientId
    content = meta.readLine(); // ctime
    // read in image ids
    QVector<QString> img_ids;
    while(!meta.atEnd()){
        content = meta.readLine();
        content.remove("\r\n");
        img_ids.push_back(content);
    }
    if (layer < 0 || layer >= img_ids.size()){
        qDebug() << "image layer out of range!";
        return QVector<imageInfo::imgMark>();
    }
    QString img_id = img_ids[layer];

    // construct the url
    QUrl url(urlbase["base2"] + urlbase["mark"]);
    QUrlQuery query;
    query.addQueryItem("imageId", img_id);
    url.setQuery(query.query());

    // construct the request
    QNetworkAccessManager local_qnam;
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("X-Auth-Token", token.toUtf8());
    // send request
    QNetworkReply* reply = local_qnam.get(request);
    // loop until get reply
    QEventLoop eventloop;
    connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
    eventloop.exec();
    // handle reply
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QVector<imageInfo::imgMark> result;
    if (status == 200){
        // normal case
        QByteArray resp = reply->readAll();
        QJsonParseError jerror;
        QJsonDocument json = QJsonDocument::fromJson(resp, &jerror);
        if (jerror.error == QJsonParseError::NoError && !json.isNull() && !json.isEmpty()){
            // parse each mark info
            QVariantList list = json.toVariant().toList();
            for (int i = 0; i < list.count(); i++){
                QVariantMap map = list[i].toMap();
                result.push_back(imageInfo::imgMark(
                        map["id"].toString(),
                        map["imageId"].toString(),
                        map["layer"].toInt(),
                        map["view"].toInt(),
                        map["topX"].toDouble(),
                        map["topY"].toDouble(),
                        map["bottomX"].toDouble(),
                        map["bottomY"].toDouble())
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
    else {
        qDebug() << "Connection error!";
        reply->deleteLater();
        return result;
    }
}

QString imageInfo::predictImageHttp(QString filepath, QString patientID){
    QFile afile(filepath);
    if (!afile.exists()){
        qDebug() << "The file doesn't exist";
        return "FILE NOT EXIST";
    }
    FILE *fp;
    QString URL = urlbase["base4"] + urlbase["predict"];
    QString req = "curl --location --request POST \"" + URL + "\"";
    req = req + " --header \"X-Auth-Token: " + token.toUtf8() + "\"";
    req = req + " --form \"patientId=" + patientID + "\"";
    req = req + " --form \"tumor_image=@" + filepath + "\"";
    char buffer[1024] = {0};

    qDebug() << req;

#ifdef Q_OS_WIN32   // Define on windows system
    fp = _popen(req.toUtf8(), "r");
#endif

#ifdef Q_OS_MACOS   // Define on MACOS system

    fp = popen(req.toUtf8(), "r");
#endif
    if (fp == NULL){
        qDebug() << "CONNECTION FAIL";
        return "CONNECTION FAIL";
    }

    while(fgets(buffer, sizeof(buffer), fp) != NULL) {
        qDebug() << buffer;
    }
    // handle the reply of server
    QString result = buffer;
    if (result == "no_tumor"){
        return result;
    }
    else if (result == "glioma_tumor"){
        return result;
    }
    else if (result == "meningioma_tumor"){
        return result;
    }
    else if (result == "pituitary_tumor"){
        return result;
    }
    else {
        return "CONNECTION FAIL";
    }
}

QString imageInfo::predictImageHttp(QString filepath){
    QFile afile(filepath);
    if (!afile.exists()){
        qDebug() << "The file doesn't exist";
        return "FILE NOT EXIST";
    }
    FILE *fp;
    QString URL = urlbase["base4"] + urlbase["predict"];
    QString req = "curl --location --request POST \"" + URL + "\"";
    req = req + " --header \"X-Auth-Token: " + token.toUtf8() + "\"";
    req = req + " --form \"tumor_image=@" + filepath + "\"";
    char buffer[1024] = {0};

    qDebug() << req;

#ifdef Q_OS_WIN32   // Define on windows system
    fp = _popen(req.toUtf8(), "r");
#endif

#ifdef Q_OS_MACOS   // Define on MACOS system

    fp = popen(req.toUtf8(), "r");
#endif
    if (fp == NULL){
        qDebug() << "CONNECTION FAIL";
        return "CONNECTION FAIL";
    }

    while(fgets(buffer, sizeof(buffer), fp) != NULL) {
        qDebug() << buffer;
    }
    // handle the reply of server
    QString result = buffer;
    result = result.replace('"', ' ');
    return result;
}

QVector<QString> imageInfo::getCtimeHttp(QString patientId){
    // construct url and query item
    QNetworkAccessManager local_qnam;
    QUrl url(urlbase["base2"] + urlbase["image"] + "/ctime");
    QUrlQuery query;
    query.addQueryItem("patientId", patientId);
    url.setQuery(query.query());
    // construct request
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("X-Auth-Token", token.toUtf8());
    // send request
    QNetworkReply* reply = local_qnam.get(request);

    QEventLoop eventloop;
    connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
    eventloop.exec();

    // handle the reply body
    if (reply->error()){
        qDebug() << reply->errorString();
        reply->deleteLater();
        return QVector<QString>();
    }
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status == 401){
        qDebug() << "Token unauthorized";
        reply->deleteLater();
        return QVector<QString>();
    }

    QVector<QString> result;
    if (status == 200){
        // normal case
        QByteArray resp = reply->readAll();
        // qDebug() << resp;
        QJsonParseError jerror;
        QJsonDocument json = QJsonDocument::fromJson(resp, &jerror);
        if (jerror.error == QJsonParseError::NoError && !json.isNull() && !json.isEmpty()){
            // parse each patient
            QVariantList list = json.toVariant().toList();
            for (int i = 0; i < list.count(); i++){
                // get each ctime
                QVariantMap map = list[i].toMap();
                result.push_back(map["ctime"].toString());
            }
        }
    }
    reply->deleteLater();
    return result;
}

void imageInfo::cancelDownload(){

}

void imageInfo::httpFinished(QNetworkReply* reply){
    // handle error
    // handle the error
    if (reply->error()){
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }
}

void imageInfo::httpReadyRead(){

}
