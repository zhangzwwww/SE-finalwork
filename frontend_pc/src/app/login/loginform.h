#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>

#include "loginchecker.h"
#include "commu/communhttp.h"
#include "commu/userinfo.h"

namespace Ui {
class LoginForm;
}

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    // objects needed for http communicator
    communhttp *commnicator;
    userinfo *user;

    explicit LoginForm(communhttp *commu, userinfo *user_info, QWidget *parent = nullptr);
    ~LoginForm();

private slots:
    void on_sign_in_button_clicked();

    void on_sign_up_button_clicked();

    void on_retrive_button_clicked();

private:
    Ui::LoginForm *ui;
    LoginChecker login_checker;
};

#endif // LOGINFORM_H
