#ifndef ERRORCODE_H
#define ERRORCODE_H

enum ERROR_CODE {
    OK                 = 0,
//    for general error code:100000~100050
    PARAM_INVALID      = 100000,
//    for login: 200000~200010
    LOGIN_ID_NOT_FOUND = 200001,
    LOGIN_ERROR_PWD    = 200002,
//    for general img processing: 200011~200040
    INPUT_IMG_INVALID  = 200011,
};

#endif // ERRORCODE_H
