#include "predict_form.h"
#include "ui_predict_form.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>

#include "struct_define.h"
#include "utils/general_util.h"

PredictForm::PredictForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PredictForm)
{
    ui->setupUi(this);
    last_folder_path = "./";
}

PredictForm::PredictForm(PredictFormParams &params, QWidget *parent) :
    QDialog (parent),
    ui(new Ui::PredictForm)
{
    ui->setupUi(this);
    user_info_ = params.user_info;
    image_manager_ = params.image_manager;
    image_manager_->setToken(user_info_._token());
}

PredictForm::~PredictForm()
{
    delete ui;
}

void PredictForm::on_openFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("open a file."),
            last_folder_path,
            tr("Image Files(*.jpg *.png *.jpeg)"));
    if (fileName.isEmpty() == true)
        return;
    QFileInfo file_info(fileName);
    last_folder_path = file_info.path();
    ui->filePathEdit->setText(fileName);
    ui->predictResultEdit->setText("");
}

void PredictForm::on_predictBtn_clicked()
{
    QString file_path = ui->filePathEdit->text();
    if (file_path.isEmpty()) {
        QMessageBox::warning(this, "error", "Please select file to predict!", QMessageBox::Yes);
        return;
    }
    QString prediction = image_manager_->predictImageHttp(file_path);
    ui->predictResultEdit->setText(prediction);
}
