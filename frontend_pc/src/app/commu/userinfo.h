#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include "communhttp.h"
#include "urlbase.h"

class userinfo
{
private:
    bool is_login;

    // Content of the user information
    // TODO: decide content of a user
    QString id;
    QString email;
    QString ctime;
    QString role;
    QString password;

    QString token;

public:
    // constructor
    userinfo();

    // judge whether the user is logged in
    bool is_logged_in();

    // update the logged in status
    void update_login(bool status);

    // get token of user locally
    QString _token();
    // get email of user locally
    QString _email();

    // handle user log in
    // input: email, password
    // return true if success
    bool log_user_in(communhttp* requester, QString email, QString pw);

    /*---------------HTTP Communication Methods----------------------*/
    // TODO: all communication methods need handle data returned by server

    // GET: get user info with a token from the server
    // return true if success
    bool get_user_info(communhttp* requester);

    // POST: register as a new user to the server
    // return true if success
    bool register_user_info(communhttp* requester, QString email, QString pw);

    // POST: get a new user token
    // return true if success
    bool get_user_token(communhttp* requester, QString email, QString pw);

    // DELETE: revoke a user token
    // return true if success
    bool delete_user_token(communhttp* requester);

    // HEAD: validate user token
    // return true if success
    bool head_token(communhttp* requester);
};

#endif // USERINFO_H
