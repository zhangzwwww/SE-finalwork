#include "loginform.h"
#include "ui_loginform.h"

#include <QMessageBox>

using std::string;

LoginForm::LoginForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    ui->id_input->setPlaceholderText(tr("账号"));
    ui->pwd_input->setPlaceholderText(tr("密码"));
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_sign_in_button_clicked()
{
    string id = ui->id_input->text().toStdString();
    string pwd = ui->pwd_input->text().toStdString();
    ERROR_CODE ret = login_checker.SignIn(id, pwd);
    if (ret == OK) {
        accept();
    } else if (ret == LOGIN_ID_NOT_FOUND) {
        QMessageBox::warning(this, tr("警告！"), tr("用户名不存在！"), QMessageBox::Yes);

        ui->id_input->clear();
        ui->pwd_input->clear();

        ui->id_input->setFocus();
    } else if (ret == LOGIN_ERROR_PWD) {
        QMessageBox::warning(this, tr("警告！"), tr("密码错误！"), QMessageBox::Yes);

        ui->pwd_input->clear();
        ui->pwd_input->setFocus();
    }
}

void LoginForm::on_sign_up_button_clicked()
{
    qDebug("TODO: sign up");
    login_checker.SignUp();
}

void LoginForm::on_retrive_button_clicked()
{
    qDebug("TODO: find my password back");
    string id = ui->id_input->text().toStdString();
    login_checker.RetrievePwd(id);
}
