#ifndef PATIENT_H
#define PATIENT_H

#include <QString>
#include <QVector>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include "communhttp.h"
#include "urlbase.h"

class patient
{
private:
    QString id;

    QString name;
    bool gender;
    QString birth;
    int age;

    // got from user object or server
    static QString token;

public:
    // create an empty patient object
    patient();

    // locally create a patient object with given info
    patient(QString name, bool gender, QString birth, int age, QString id="");

    // locally set a patient object with given info
    void set_patient_info(QString name, bool gender, QString birth, int age);

    // set the token for communication
    static void set_token(QString t);

    // get patient information of the patient
    QString _id();
    QString _name();
    bool _gender();
    QString _birth();
    int _age();

    /*---------------HTTP Communication Methods----------------------*/

    // get all patient info on the server
    // return a list of patients if success
    static QVector<patient> http_get_all_patient(communhttp* req);

    // post a new patient to the server
    // return true if success
    static bool http_create_patient(communhttp* req, QString name, bool gender, QString birth, int age);

    // get a patient by id
    // return a patient object if success
    static patient http_getPatient_byId(communhttp *req, QString id);

    // modify a patient by id
    // return true if success
    static bool http_modifyPatient_byId(communhttp *req, QString id, QString name, bool gender, QString birth, int age);

    // delete a patient by id
    // return true if success
    static bool http_deletePatient_byId(communhttp *req, QString id);
};

#endif // PATIENT_H
