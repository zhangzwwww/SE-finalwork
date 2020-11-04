#ifndef LOGINCHECKER_H
#define LOGINCHECKER_H

#include <string>

#include "errorcode.h"

using std::string;

class LoginChecker
{
public:
    LoginChecker();
    ~LoginChecker();

    ERROR_CODE SignIn(string id, string pwd);
    ERROR_CODE SignUp();
    ERROR_CODE RetrievePwd(string id);
};

#endif // LOGINCHECKER_H
