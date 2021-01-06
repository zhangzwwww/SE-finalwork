#ifndef DOWNLOAD_FORM_H
#define DOWNLOAD_FORM_H

#include <QDialog>
#include <QComboBox>
#include <QVector>

#include "commu/communhttp.h"
#include "commu/patient.h"
#include "commu/userinfo.h"
#include "commu/imageinfo.h"

namespace Ui {
class DownloadForm;
}

struct DownloadFormParams {
    userinfo user_info;
    imageInfo *image_manager;
    QVector<patient> patients;
};

class DownloadForm : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadForm(QWidget *parent = nullptr);
    explicit DownloadForm(QComboBox *patient_id_combo, QComboBox *image_name_combo, QWidget *parent = nullptr);
    explicit DownloadForm(DownloadFormParams &params, QWidget *parent = nullptr);
    ~DownloadForm();

private slots:
    void on_openFileBtn_clicked();

    void on_downloadFileBtn_clicked();

    void on_patientIDSelector_currentIndexChanged(int index);

private:
    Ui::DownloadForm *ui;
    communhttp communicator_;
    userinfo user_info_;
    QVector<patient> patients_;
    imageInfo *image_manager_;
};

#endif // UPLOAD_FORM_H
