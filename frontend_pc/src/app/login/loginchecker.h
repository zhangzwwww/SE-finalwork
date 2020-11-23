#ifndef LOGINCHECKER_H
#define LOGINCHECKER_H

#include <string>
#include <QString>

#include "../errorcode.h"
#include "../commu/communhttp.h"
#include "../commu/userinfo.h"

using std::string;

class LoginChecker
{
public:
    LoginChecker();
    ~LoginChecker();

    ERROR_CODE SignIn(QString id, QString pwd, communhttp *commu, userinfo *user);
    ERROR_CODE SignUp(QString id, QString pwd, communhttp *commu, userinfo *user);
    ERROR_CODE RetrievePwd(QString id);
};

#endif // LOGINCHECKER_H
