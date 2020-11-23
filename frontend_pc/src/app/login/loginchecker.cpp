#include "loginchecker.h"

LoginChecker::LoginChecker()
{

}

LoginChecker::~LoginChecker() {

}

ERROR_CODE LoginChecker::SignIn(QString id, QString pwd, communhttp *commu, userinfo *user) {
//    TODO: check if id exists
    if (id.isEmpty()) {
        return LOGIN_ID_NOT_FOUND;
    }
//    TODO: check if pwd is correct
    if (pwd.isEmpty()) {
        return LOGIN_ERROR_PWD;
    }
    // try to log in the user
    if (user->log_user_in(commu, id, pwd)){
        return OK;
    }
    else {
        return LOGIN_ERROR_PWD;
    }
}

ERROR_CODE LoginChecker::SignUp() {
    return OK;
}

ERROR_CODE LoginChecker::RetrievePwd(QString id) {
    return OK;
}
