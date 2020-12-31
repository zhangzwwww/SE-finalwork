#include "upload_form.h"
#include "ui_upload_form.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

#include "struct_define.h"
#include "utils/general_util.h"

UploadForm::UploadForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadForm)
{
    ui->setupUi(this);
}

UploadForm::UploadForm(QComboBox *patient_id_combo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadForm)
{
    ui->setupUi(this);
    int count = int(patient_id_combo->count());
    for (int i = 0; i < count; i++) {
        ui->patientIDSelector->addItem(patient_id_combo->itemText(i));
    }
}

UploadForm::UploadForm(UploadFormParams &params, QWidget *parent) :
    QDialog (parent),
    ui(new Ui::UploadForm)
{
    ui->setupUi(this);
    user_info_ = params.user_info;
    patients_ = params.patients;
    image_manager_ = params.image_manager;
    image_manager_->setToken(user_info_._token());
    if (patients_.empty()) {
        patient::set_token(user_info_._token());
        patients_ = patient::http_get_all_patient(&communicator_);
    }
    for (patient &pat:patients_) {
//        ui->patientIDSelector->addItem(QString("[%1]%2").arg(pat._name()).arg(pat._id()));
        ui->patientIDSelector->addItem(pat._name());
    }
}

UploadForm::~UploadForm()
{
    delete ui;
}

void UploadForm::on_openFileBtn_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, QString(tr("Open DICOM Image")));

    if (fileName.isEmpty() == true)
        return;

    ui->filePathEdit->setText(fileName);
    ui->imageNameEdit->setText(GetFileName(fileName));
}

void UploadForm::on_uploadFileBtn_clicked()
{
//    TODO: upload file
    if (ui->filePathEdit->text().isEmpty()) {
        QMessageBox::warning(this, "w", "Please select folder to upload!", QMessageBox::Yes);
        return;
    }
    if (ui->imageNameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "w", "Please input image name!", QMessageBox::Yes);
        return;
    }
//    UploadFileParams params;
    QString file_path = ui->filePathEdit->text();
    int cur_patient_index = ui->patientIDSelector->currentIndex();
    QString patient_id = patients_[cur_patient_index]._id();
    QString image_name = ui->imageNameEdit->text();
//    if upload succeed
    image_manager_->setFilePath(file_path);
    image_manager_->uploadFolerImage(patient_id, image_name, file_path);
//    image_manager_->uploadImageHttp(patient_id, image_name, file_path);
    accept();
}

void UploadForm::on_imageNameEdit_editingFinished()
{
    if (ui->imageNameEdit->hasFocus()) {
        qDebug()<<"editing finished";
        on_uploadFileBtn_clicked();
    }
}
