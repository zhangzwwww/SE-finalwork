#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "vtkSmartPointer.h"

class vtkImageViewer2;
class vtkImageData;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void init_VTKView();
    void showImage();

private slots:
    void on_loadImage_clicked();
    void on_Volume_clicked();

    void on_pushButton_4_clicked();

    void on_in_fusion_2_opacity_valueChanged(int value);

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_start_smoothing_button_clicked();

    void on_detect_edge_button_clicked();

    void on_start_thresholding_button_clicked();

private:
    vtkSmartPointer<vtkImageViewer2> riw_[3];
    vtkSmartPointer<vtkImageData> image_;
    int dims[3];

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
