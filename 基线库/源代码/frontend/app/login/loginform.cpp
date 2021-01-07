#include "loginform.h"
#include "ui_loginform.h"
#include "signup_form.h"

#include <QMessageBox>
#include <QSizePolicy>

using std::string;

LoginForm::LoginForm(communhttp *commu, userinfo *user_info, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    ui->id_input->setPlaceholderText("email address");
    ui->pwd_input->setPlaceholderText("password");
    ui->id_input->setFocus();
//    hide forget pwd button

    // initialize communication object
    this->user = user_info;
    this->commnicator = commu;
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_sign_in_button_clicked()
{
    QString id = ui->id_input->text();
    QString pwd = ui->pwd_input->text();
    ERROR_CODE ret = login_checker.SignIn(id, pwd, this->commnicator, this->user);
    if (ret == OK) {
        accept();
    } else if (ret == PARAM_INVALID) {
        QMessageBox::warning(this, tr("Warning"), tr("Wrong ID or Password"), QMessageBox::Yes);

        ui->id_input->clear();
        ui->pwd_input->clear();

        ui->id_input->setFocus();
    } else if (ret == LOGIN_ERROR_PWD) {
        QMessageBox::warning(this, tr("Error"), tr("Wrong ID or Password"), QMessageBox::Yes);

        ui->pwd_input->clear();
        ui->pwd_input->setFocus();
    }
}

void LoginForm::on_sign_up_button_clicked()
{
    qDebug("TODO: sign up");
    SignUpForm sign_up_form(commnicator, user);
    sign_up_form.exec();
    ui->id_input->clear();
    ui->pwd_input->clear();
    ui->id_input->setFocus();
//    QString id = ui->id_input->text();
//    QString pwd = ui->pwd_input->text();
//    ERROR_CODE ret = login_checker.SignUp(id, pwd, this->commnicator, this->user);
//    if (ret == OK){
//        QMessageBox::information(this, tr("w"), tr("Registration Succeeds!"), QMessageBox::Yes);
//        ui->pwd_input->clear();
//        ui->pwd_input->setFocus();
//    }
//    else if (ret == LOGIN_ID_NOT_FOUND){
//        QMessageBox::warning(this, tr("w"), tr("ID cannot be empty!"), QMessageBox::Yes);
//        ui->pwd_input->clear();
//        ui->id_input->setFocus();
//    }
//    else if (ret == LOGIN_ERROR_PWD){
//        QMessageBox::warning(this, tr("w"), tr("Password cannot be empty!"), QMessageBox::Yes);
//        ui->pwd_input->setFocus();
//    }
//    else if (ret == PARAM_INVALID){
//        QMessageBox::warning(this, tr("w"), tr("ID already exists!"), QMessageBox::Yes);
//        ui->id_input->clear();
//        ui->pwd_input->clear();
//        ui->id_input->setFocus();
//    }
//    else {
//        QMessageBox::warning(this, tr("w"), tr("Registration ERROR!"), QMessageBox::Yes);
//        ui->id_input->clear();
//        ui->pwd_input->clear();
//        ui->id_input->setFocus();
//    }
}

