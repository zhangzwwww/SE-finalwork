#include "loginform.h"
#include "ui_loginform.h"

#include <QMessageBox>

using std::string;

LoginForm::LoginForm(communhttp *commu, userinfo *user_info, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    ui->id_input->setPlaceholderText(tr("账号"));
    ui->pwd_input->setPlaceholderText(tr("密码"));

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
        QMessageBox::warning(this, tr("w"), tr("Wrong ID or Password"), QMessageBox::Yes);

        ui->id_input->clear();
        ui->pwd_input->clear();

        ui->id_input->setFocus();
    } else if (ret == LOGIN_ERROR_PWD) {
        QMessageBox::warning(this, tr("1"), tr("Wrong ID or Password"), QMessageBox::Yes);

        ui->pwd_input->clear();
        ui->pwd_input->setFocus();
    }
}

void LoginForm::on_sign_up_button_clicked()
{
    qDebug("TODO: sign up");
    QString id = ui->id_input->text();
    QString pwd = ui->pwd_input->text();
    ERROR_CODE ret = login_checker.SignUp(id, pwd, this->commnicator, this->user);
    if (ret == OK){
        QMessageBox::information(this, tr("w"), tr("Registration Succeeds!"), QMessageBox::Yes);
        ui->pwd_input->clear();
        ui->pwd_input->setFocus();
    }
    else if (ret == LOGIN_ID_NOT_FOUND){
        QMessageBox::warning(this, tr("w"), tr("ID cannot be empty!"), QMessageBox::Yes);
        ui->pwd_input->clear();
        ui->id_input->setFocus();
    }
    else if (ret == LOGIN_ERROR_PWD){
        QMessageBox::warning(this, tr("w"), tr("Password cannot be empty!"), QMessageBox::Yes);
        ui->pwd_input->setFocus();
    }
    else if (ret == PARAM_INVALID){
        QMessageBox::warning(this, tr("w"), tr("ID already exists!"), QMessageBox::Yes);
        ui->id_input->clear();
        ui->pwd_input->clear();
        ui->id_input->setFocus();
    }
    else {
        QMessageBox::warning(this, tr("w"), tr("Registration ERROR!"), QMessageBox::Yes);
        ui->id_input->clear();
        ui->pwd_input->clear();
        ui->id_input->setFocus();
    }
}

void LoginForm::on_retrive_button_clicked()
{
    qDebug("TODO: find my password back");
    QString id = ui->id_input->text();
    login_checker.RetrievePwd(id);
}
