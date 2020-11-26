#include "loginchecker.h"

LoginChecker::LoginChecker()
{

}

LoginChecker::~LoginChecker() {

}

ERROR_CODE LoginChecker::SignIn(QString id, QString pwd, communhttp *commu, userinfo *user) {
    if (id.isEmpty()) {
        return PARAM_INVALID;
    }
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

ERROR_CODE LoginChecker::SignUp(QString id, QString pwd, communhttp *commu, userinfo *user) {
    if (id.isEmpty()){
        return LOGIN_ID_NOT_FOUND;
    }
    if (pwd.isEmpty()){
        return LOGIN_ERROR_PWD;
    }
    // try to register in the user
    if (user->register_user_info(commu, id, pwd)){
        return OK;
    }
    return PARAM_INVALID;
}

ERROR_CODE LoginChecker::RetrievePwd(QString id) {
    return OK;
}
