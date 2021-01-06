#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QUrl>
#include <QDir>
#include <QUrlQuery>
#include <QProgressDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <stdlib.h>

#include "communhttp.h"
#include "urlbase.h"


class ProgressDialog : public QProgressDialog{
    Q_OBJECT

public:
    explicit ProgressDialog(const QUrl &url, QWidget *parent=nullptr, QString progress="Download");
    ~ProgressDialog();

public slots:
    void networkReplyProgress(qint64 bytesRead, qint64 totalBytes);
};


class imageInfo : public QObject
{
    Q_OBJECT
private:
    // the path where user save the image data
    static QString save_path;
    // got from user object or server
    static QString token;

    // the communicator
    QNetworkAccessManager qnam;
    // the folder path for a patient and ctime
    QString folder_path;
    // the parent widget
    // QWidget* parent;

private slots:
    void downloadFile(QNetworkReply*);
    void uploadFile(QString);
    void cancelDownload();
    void httpFinished(QNetworkReply*);
    void httpReadyRead();

signals:
    void sendreq(QString);

public:
    // Define a struct for image mark
    struct imgMark{
        QString markId;
        QString imgId;
        int layer;
        int view;
        double topX;
        double topY;
        double bottomX;
        double bottomY;

        imgMark(QString mid, QString imgid, int l, int v, double tx, double ty, double bx, double by){
            this->markId = mid;
            this->imgId = imgid;
            this->layer = l;
            this->view = v;
            this->topX = tx;
            this->topY = ty;
            this->bottomX = bx;
            this->bottomY = by;
        }

        imgMark(){
        }
    };

    // constructor and destructor
    explicit imageInfo(QObject *parent = nullptr);
    ~imageInfo();

    // set file path that will save images
    static void setFilePath(QString path);
    // set user token used in http communication
    static void setToken(QString token);

    // Asynchronously get info and files through http requests
    // INPUT: patientID and ctime needed for searching data
    void getImagesHttp(QString patientID, QString ctime);

    // Upload image folder to the server
    // INPUT: patientId, ctime, folderpath
    void uploadFolerImage(QString patientId, QString ctime, QString folderpath);

    // Upload image data to the server
    // INPUT: patientId, ctime and Filepath
    void uploadImageHttp(QString patientId, QString ctime, QString filepath);

    // Upload a marked image data to the server
    // INPUT: folderpath containting meta file, 2 INT for image level and view type, 4 DOUBLEs for mark position
    // RETURN: return true if upload success
    int uploadImgMark(QString folderpath, int level, int view, double topX, double topY, double bottomX, double bottomY);

    // Get all marked information
    // INPUT: folderpath containing meta file, INT for the index of image
    // RETURN: a vector containing all marks
    QVector<imgMark> getAllMarks(QString folderpath, int level);

    // Upload image to sever and get prediction result
    // INPUT: Filepath, PatientId
    // NORMAL OUTPUT: class of image
    // FAIL CASE: "FILE NOT EXIST"/"CONNECTION FAIL"
    QString predictImageHttp(QString filepath, QString patientID);
    QString predictImageHttp(QString filepath);

    // Get image ctimes given patientId
    // INPUT: patientId
    QVector<QString> getCtimeHttp(QString patientId);


};

#endif // IMAGEINFO_H
