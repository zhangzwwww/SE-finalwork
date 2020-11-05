#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>

#include "loginchecker.h"

namespace Ui {
class LoginForm;
}

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
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
