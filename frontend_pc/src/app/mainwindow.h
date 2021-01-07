#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// #include "struct_define.h"
#include <vector>

// QT
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>


// VTK
#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);


#include "vtkSmartPointer.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageViewer2.h"
#include "vtkCamera.h"
#include "vtkAxisActor.h"
#include "vtkNIFTIImageReader.h"

#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkRendererCollection.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtk_add_seed_callback.h>

#include <stdio.h>
#include <stdlib.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include "vtkResliceImageViewer.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkResliceCursorThickLineRepresentation.h"
#include "vtkResliceCursorWidget.h"
#include "vtkResliceCursorActor.h"
#include "vtkResliceCursorPolyDataAlgorithm.h"
#include "vtkResliceCursor.h"
#include "vtkDICOMImageReader.h"
#include "vtkMetaImageReader.h"
#include "vtkCellPicker.h"
#include "vtkProperty.h"
#include "vtkPlane.h"
#include "vtkImageData.h"
#include "vtkCommand.h"
#include "vtkPlaneSource.h"
#include "vtkLookupTable.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkInteractorStyleImage.h"
#include "vtkImageSlabReslice.h"
#include "vtkBoundedPlanePointPlacer.h"
#include "vtkDistanceWidget.h"
#include "vtkDistanceRepresentation.h"
#include "vtkHandleRepresentation.h"
#include "vtkResliceImageViewerMeasurements.h"
#include "vtkDistanceRepresentation2D.h"
#include "vtkPointHandleRepresentation3D.h"
#include "vtkPointHandleRepresentation2D.h"

#include "vtkImageGaussianSmooth.h"
#include "vtkImageMedian3D.h"
#include "vtkImageConvolve.h"
#include <vtkPointPicker.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkImageActor.h>
#include <vtkSphereSource.h>

//
#include <vtkImageThreshold.h>
#include <vtkPolyDataMapper.h>
#include <vtkImageGradientMagnitude.h>
#include <vtkImageCast.h>


// ITK
#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>
#include <itkVTKImageToImageFilter.h>
#include <itkAddImageFilter.h>
#include <itkMultiplyImageFilter.h>

#include <vtkImageSliceMapper.h>
#include <vtkImageSlice.h>
#include <vtkImageStack.h>

#include <vtkSmartPointer.h>
#include <itkImage.h>

#ifdef Q_OS_MACOS   // Define on MACOS system
#include "commu/communhttp.h"
#include "commu/userinfo.h"
#include "commu/patient.h"
#include "commu/patient_form.h"
#include "commu/imageinfo.h"
#include "utils/general_util.h"
#include "load/upload_form.h"
#include "load/download_form.h"
#include "struct_define.h"
#include "registration/registration_worker.h"
#include "voxel2mesh/voxel2mesh.h"
#include "segmentation/segmentation_worker.h"
#include "predict/predict_form.h"
#include "utils/vtk_point_picker_callback.h"
#endif

#ifdef Q_OS_WIN32   // Define on windows system
#include "commu/communhttp.h"
#include "commu/userinfo.h"
#include "commu/patient.h"
#include "commu/patient_form.h"
#include "commu/imageinfo.h"
#include "utils/general_util.h"
#include "load/upload_form.h"
#include "load/download_form.h"
#include "struct_define.h"
#include "registration_worker.h"
#include "voxel2mesh.h"
#include "segmentation_worker.h"
#include "predict/predict_form.h"
#include "utils/vtk_point_picker_callback.h"
#endif

using std::vector;

class vtkImageViewer2;
class vtkRenderer;
class vtkImageData;
class vtkVolume;
class vtkImageStack;
class vtkActor;

struct ImageDataItem {
    QString image_name;
    QString image_path;
    vtkSmartPointer<vtkImageData> image_data;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // define a http communicator
    communhttp communicator;
    // define the user object
    userinfo user;
    // define the image requester
    imageInfo image_requester;

    // utility class
    class vtkSharedWindowLevelCallback : public vtkCommand{
    public:
        static vtkSharedWindowLevelCallback* New();
        void Execute(vtkObject* caller, unsigned long ev, void* callData);
        vtkSharedWindowLevelCallback();
        vtkImageViewer2* view[3];
    };

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

private:
    void init_views();
    void show_image();
    void clean_view4();
    void update_data_manager();
    void update_patients();

    void setMandatoryField(QWidget* widget, bool bEnabled);
    void setQStyleSheetField(QWidget* widget, const char* fieldName, bool bEnabled);
    

private slots:
    void load_image();
    void volume_rendering(bool status);
    void view_zoom_to_fit();
    void view_full_screen(bool full_status);
    void view_change_slice();

    void start_registration();
    void workerProgressUpdate(float progress);
    void workerIsDone(itk::DataObject::Pointer data);
    void start_fusion();
    void generate_surface();
    void clean_actors();
    void clear_manager();
    void slidervalueChanged(int pos);
    void start_segmentation();
    void start_add_seeds();

    vtkSmartPointer<vtkImageData> image_detect_edge(vtkImageData* input_image);
    vtkSmartPointer<vtkImageData> image_threshold(vtkImageData* input_image, ThresholdingParams params);
    vtkSmartPointer<vtkImageData> image_smoothing(vtkImageData* input_image, SmoothingParams params);


//    void on_pushButton_4_clicked();

//    void on_in_fusion_2_opacity_valueChanged(int value);

//    void on_pushButton_5_clicked();

//    void on_pushButton_7_clicked();

//    void on_pushButton_6_clicked();

//    void on_pushButton_clicked();

    void on_start_smoothing_button_clicked();

    void on_detect_edge_button_clicked();

    void on_start_thresholding_button_clicked();


    void on_data_manager_itemClicked(QTreeWidgetItem *item, int column);

    void on_addPatientBtn_clicked();

    void on_action_upload_file_triggered();

    void on_action_download_file_triggered();

    void on_patientSelector_currentTextChanged(const QString &arg1);

    void on_action_predict_triggered();

    void on_start_mark_btn_clicked();

    void on_refresh_mark_btn_clicked();

    void on_upload_mark_btn_clicked();

private:
    vtkSmartPointer<vtkImageViewer2> riw_[3];
    vtkSmartPointer<vtkRenderer> renderer3D_;
    vtkSmartPointer<vtkVolume> volume_;

    //vtkSmartPointer<vtkRenderer> m_Renderer2D[3];
    //vtkSmartPointer<vtkImageStack> m_ImageStack2D[3];

    vector<vtkSmartPointer<vtkImageData>> vtk_image_collection_;
    vector<itk::Image<float, 3>::Pointer> itk_image_collection_;

    vector<vector<ImageDataItem>> image_tree_;
    int cur_selected_image_ind_[2]{ -1 };

    QVector<patient> patients_;
    vtkSmartPointer< vtkPointPickerCallback > point_picker_cbk;
    vtkSmartPointer< vtkAddSeedCallback > add_seed_cbk;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
