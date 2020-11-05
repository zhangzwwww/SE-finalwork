#include "loginchecker.h"

LoginChecker::LoginChecker()
{

}

LoginChecker::~LoginChecker() {

}

ERROR_CODE LoginChecker::SignIn(string id, string pwd) {
//    TODO: check if id exists
    if (id.empty()) {
        return LOGIN_ID_NOT_FOUND;
    }
//    TODO: check if pwd is correct
    if (pwd.empty()) {
        return LOGIN_ERROR_PWD;
    }
    return OK;
}

ERROR_CODE LoginChecker::SignUp() {
    return OK;
}

ERROR_CODE LoginChecker::RetrievePwd(string id) {
    return OK;
}
