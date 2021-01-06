#include "download_form.h"
#include "ui_download_form.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QVector>

#include "struct_define.h"
#include "utils/general_util.h"

DownloadForm::DownloadForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadForm)
{
    ui->setupUi(this);
}

DownloadForm::DownloadForm(QComboBox *patient_id_combo, QComboBox *image_name_combo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadForm)
{
    ui->setupUi(this);
    int count = int(patient_id_combo->count());
    for (int i = 0; i < count; i++) {
        ui->patientIDSelector->addItem(patient_id_combo->itemText(i));
    }
    count = int(image_name_combo->count());
    for (int i = 0; i < count; i++) {
        ui->imageNameSelector->addItem(image_name_combo->itemText(i));
    }
}

DownloadForm::DownloadForm(DownloadFormParams &params, QWidget *parent) :
    QDialog (parent),
    ui(new Ui::DownloadForm)
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
    for (patient &pat: patients_) {
//        ui->patientIDSelector->addItem(QString("[%1]%2").arg(pat._name()).arg(pat._id()));
        ui->patientIDSelector->addItem(pat._name());
    }
}

DownloadForm::~DownloadForm()
{
    delete ui;
}

void DownloadForm::on_openFileBtn_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, QString(tr("Select DICOM Image Save Folder")));

    if (fileName.isEmpty() == true)
        return;

    ui->filePathEdit->setText(fileName);
}

void DownloadForm::on_downloadFileBtn_clicked()
{
//    TODO: download file
    QString file_path = ui->filePathEdit->text();
    if (file_path.isEmpty()) {
        QMessageBox::warning(this, "w", "Please select folder to upload!", QMessageBox::Yes);
        return;
    }
    int cur_patient_index = ui->patientIDSelector->currentIndex();
    if (cur_patient_index >= patients_.size()) {
        QMessageBox::warning(this, "w", "Invalid patient id!", QMessageBox::Yes);
        return;
    }
    QString patient_id = patients_[cur_patient_index]._id();
    QString ctime = ui->imageNameSelector->currentText();
    if (ctime.isEmpty()) {
        QMessageBox::warning(this, "w", "Invalid ctime!", QMessageBox::Yes);
        return;
    }
    image_manager_->setFilePath(file_path);
    image_manager_->getImagesHttp(patient_id, ctime);
//    DownloadFileParams params;
//    params.save_path = ui->filePathEdit->text().toStdString();
//    params.patient_id = ui->patientIDSelector->currentText().toStdString();
//    params.image_name = ui->imageNameSelector->currentText().toStdString();
//    if upload succeed
    accept();
}

void DownloadForm::on_patientIDSelector_currentIndexChanged(int index)
{
//    TODO: update image name list
    if (index >= patients_.size()) {
        return;
    }
    ui->imageNameSelector->clear();
    QVector<QString> image_names = image_manager_->getCtimeHttp(patients_[index]._id());
    if (image_names.isEmpty()) {
        return;
    }
    for (QString &name:image_names) {
        ui->imageNameSelector->addItem(name);
    }
}
