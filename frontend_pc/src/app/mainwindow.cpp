#include "mainwindow.h"
#include "ui_mainwindow.h"

// QT
#include <QFileDialog>
#include <QMessageBox>

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

#include <vtkImageThreshold.h>


#include "struct_define.h"

#include "RegistrationWorker.h"


// ITK
#include <itkImage.h>
#include <itkImageToVTKImageFilter.h> 
#include <itkVTKImageToImageFilter.h> 


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	image_itk_(nullptr), image_vtk_(nullptr)
{
	ui->setupUi(this);

    init_VTKView();

    connect(ui->actionLoadImage, SIGNAL(triggered()), this, SLOT(load_image()));
	connect(ui->actionVolume_Rendering, SIGNAL(triggered()), this, SLOT(volume_rendering()));

	connect(ui->zoomBtn1, SIGNAL(clicked()), this, SLOT(zoom_to_fit()));
	connect(ui->zoomBtn2, SIGNAL(clicked()), this, SLOT(zoom_to_fit()));
	connect(ui->zoomBtn3, SIGNAL(clicked()), this, SLOT(zoom_to_fit()));
	
	connect(ui->fullScreenBtn1, SIGNAL(clicked(bool)), this, SLOT(view1_full_screen(bool)));
	connect(ui->fullScreenBtn2, SIGNAL(clicked(bool)), this, SLOT(view2_full_screen(bool)));
	connect(ui->fullScreenBtn3, SIGNAL(clicked(bool)), this, SLOT(view3_full_screen(bool)));
	connect(ui->fullScreenBtn4, SIGNAL(clicked(bool)), this, SLOT(view4_full_screen(bool)));




//    TODO: make sure the number of spinbox/lineedit is legal
//    AlgorithmParams
//    FusionParams
//    RegistrationParams
//    ProcessingParams
//    MedianFilterParams
//    GaussianParams
//    PolygonSmoothingParams
}

MainWindow::~MainWindow()
{
    delete ui;
	
}

void MainWindow::init_VTKView()
{

	for (int i = 0; i < 3; i++)
	{
		riw_[i] = vtkSmartPointer< vtkImageViewer2 >::New();
		//riw[i]->SetLookupTable(riw[0]->GetLookupTable());
	}

	riw_[0]->SetRenderWindow(this->ui->view1->GetRenderWindow());
	riw_[1]->SetRenderWindow(this->ui->view2->GetRenderWindow());
	riw_[2]->SetRenderWindow(this->ui->view3->GetRenderWindow());

	riw_[0]->SetupInteractor(
		this->ui->view1->GetRenderWindow()->GetInteractor());
	riw_[1]->SetupInteractor(
		this->ui->view2->GetRenderWindow()->GetInteractor());
	riw_[2]->SetupInteractor(
		this->ui->view3->GetRenderWindow()->GetInteractor());

	renderer3D_ = vtkSmartPointer<vtkRenderer>::New();
	renderer3D_->SetBackground(1, 1, 1);
	renderer3D_->SetBackground2(0.5, 0.5, 0.5);
	renderer3D_->SetGradientBackground(1);

	this->ui->view4->GetRenderWindow()->AddRenderer(renderer3D_);

	//this->ui->view1->show();
	//this->ui->view2->show();
	//this->ui->view3->show();

}

void MainWindow::load_image()
{
	QString fileName = QFileDialog::getExistingDirectory(this, QString(tr("Open DICOM Image")));

	if (fileName.isEmpty() == true)
		return;

	QByteArray ba = fileName.toLocal8Bit();
	const char* fileName_str = ba.data();

	RegistrationWorker worker;
	image_itk_ = worker.readImageDICOM(fileName_str);

	if (image_itk_ == nullptr)
	{
		QMessageBox::warning(nullptr,
			tr("Read Image Error"),
			tr("Read image error, Please read the correct Image."),
			QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return;
	}

	using FilterType = itk::ImageToVTKImageFilter<itk::Image<float, 3>>;
	FilterType::Pointer filter = FilterType::New();
	filter->SetInput(image_itk_);

	try
	{
		filter->Update();
	}
	catch (itk::ExceptionObject & error)
	{
		QMessageBox::warning(nullptr,
			tr("Read Image Error"),
			tr("Read image error, Please read the correct Image."),
			QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

	}

	// clean the current volume
	this->clean_reconstruction();

	image_vtk_ = filter->GetOutput();

	this->showImage();
	//dataReady = true;
}


void MainWindow::showImage()
{
	//vtkSmartPointer< vtkCamera > camera = vtkSmartPointer< vtkCamera >::New();
	//vtkSmartPointer<vtkFourViewerCallback> cbk =
	//	vtkSmartPointer<vtkFourViewerCallback>::New();

	double range[2];
	int dims[3];
	image_vtk_->GetScalarRange(range);
	image_vtk_->GetDimensions(dims);

	for (int i = 0; i < 3; i++)
	{
		riw_[i]->SetInputData(image_vtk_);
		riw_[i]->SetSliceOrientation(i);
		riw_[i]->SetSlice(dims[i] / 2);

		riw_[i]->SetColorWindow(range[1] - range[0]);
		riw_[i]->SetColorLevel((range[0] + range[1]) / 2.0);

	}

	//riw_[i]->Render();
	//riw_[i]->GetRenderer()->ResetCamera();

}

void MainWindow::volume_rendering() 
{
	if (image_vtk_ == nullptr)
	{
		QMessageBox::warning(nullptr,
			tr("Error"),
			tr("Please read the image first."),
			QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		
		return;
	}

	if (volume_ == nullptr)
	{
		volume_ = vtkSmartPointer<vtkVolume>::New();
	}

	vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper =
		vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	volumeMapper->SetInputData(image_vtk_);

	volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance() / 4);
	volumeMapper->SetAutoAdjustSampleDistances(0);
	volumeMapper->SetImageSampleDistance(2);

	vtkSmartPointer<vtkVolumeProperty> volumeProperty =
		vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->ShadeOn();
	volumeProperty->SetAmbient(.1);
	volumeProperty->SetDiffuse(.9);
	volumeProperty->SetSpecular(.2);
	volumeProperty->SetSpecularPower(10);

	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
		vtkSmartPointer<vtkPiecewiseFunction>::New();
	vtkSmartPointer<vtkColorTransferFunction> colorFun =
		vtkSmartPointer<vtkColorTransferFunction>::New();

	compositeOpacity->AddPoint(-3024, 0, 0.5, 0.0);
	compositeOpacity->AddPoint(-16, 0, .49, .61);
	compositeOpacity->AddPoint(641, .72, .5, 0.0);
	compositeOpacity->AddPoint(3071, .71, 0.5, 0.0);

	colorFun->AddRGBPoint(-3024, 0, 0, 0, 0.5, 0.0);
	colorFun->AddRGBPoint(-16, 0.73, 0.25, 0.30, 0.49, .61);
	colorFun->AddRGBPoint(641, .90, .82, .56, .5, 0.0);
	colorFun->AddRGBPoint(3071, 1, 1, 1, .5, 0.0);

	volumeProperty->SetScalarOpacity(compositeOpacity); 
	volumeProperty->SetColor(colorFun);

	
	volume_->SetMapper(volumeMapper);
	volume_->SetProperty(volumeProperty);

	renderer3D_->AddVolume(volume_);
	renderer3D_->ResetCamera();
	this->ui->view4->GetRenderWindow()->Render();
}


void MainWindow::clean_reconstruction()
{
	if (volume_ != nullptr)
	{
		renderer3D_->RemoveVolume(volume_);
	}

}

void MainWindow::zoom_to_fit()
{
	for (int i = 0; i < 3; i++)
	{
		riw_[i]->GetRenderer()->ResetCamera();
		riw_[i]->Render();
	}

}

void MainWindow::view1_full_screen(bool full_status)
{
	if (full_status)
	{
		this->ui->view1Widget->show();

		this->ui->view2Widget->hide();
		this->ui->view3Widget->hide();
		this->ui->view4Widget->hide();
	}
	else {
		this->ui->view1Widget->show();
		this->ui->view2Widget->show();
		this->ui->view3Widget->show();
		this->ui->view4Widget->show();
	}
}


void MainWindow::view2_full_screen(bool full_status)
{
	if (full_status)
	{
		this->ui->view2Widget->show();

		this->ui->view1Widget->hide();
		this->ui->view3Widget->hide();
		this->ui->view4Widget->hide();
	}
	else {
		this->ui->view1Widget->show();
		this->ui->view2Widget->show();
		this->ui->view3Widget->show();
		this->ui->view4Widget->show();
	}
}



void MainWindow::view3_full_screen(bool full_status)
{
	if (full_status)
	{
		this->ui->view3Widget->show();

		this->ui->view2Widget->hide();
		this->ui->view1Widget->hide();
		this->ui->view4Widget->hide();
	}
	else {
		this->ui->view1Widget->show();
		this->ui->view2Widget->show();
		this->ui->view3Widget->show();
		this->ui->view4Widget->show();
	}
}



void MainWindow::view4_full_screen(bool full_status)
{
	if (full_status)
	{
		this->ui->view4Widget->show();

		this->ui->view2Widget->hide();
		this->ui->view3Widget->hide();
		this->ui->view1Widget->hide();
	}
	else {
		this->ui->view1Widget->show();
		this->ui->view2Widget->show();
		this->ui->view3Widget->show();
		this->ui->view4Widget->show();
	}
}

void MainWindow::image_threshold(vtkImageData* input_image, 
	vtkImageData* output_image, ThresholdingParams params)
{
	vtkSmartPointer<vtkImageThreshold> thresholdFilter = vtkSmartPointer<vtkImageThreshold>::New();
	
	thresholdFilter->SetInputData(input_image);
	thresholdFilter->ThresholdBetween(params.lower_value, params.upper_value);
	
	//thresholdFilter->ReplaceInOn();//阈值内的像素值替换
	thresholdFilter->ReplaceOutOn();//阈值外的像素值替换

	thresholdFilter->SetInValue(1);//阈值内像素值全部替换成1
	thresholdFilter->SetOutValue(0);//阈值外像素值全部替换成0

	thresholdFilter->Update();

	output_image = thresholdFilter->GetOutput();
}



void MainWindow::on_pushButton_4_clicked()
{
    // TODO: start fusion
    FusionParams fusion_params;
    //fusion_params.img0 = ui->in_fusion_0_img0->currentText().toStdString();
   // fusion_params.img1 = ui->in_fusion_1_img1->currentText().toStdString();
    //fusion_params.opacity = double(ui->in_fusion_2_opacity->value())/100.0;
    //fusion_params.fused_img_name = ui->in_fusion_3_fused_img_name->text().toStdString();
}

void MainWindow::on_in_fusion_2_opacity_valueChanged(int value)
{
    //ui->out_fusion_0_opacity->setText(QString::number(double(value)/100.0));
}

void MainWindow::on_pushButton_5_clicked()
{
//    TODO: Load fixed image
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("open a file."),
            "D:/",
            tr("All files(*.*)"));
    //ui->in_registration_0_fixed_img_name->setText(fileName);
}

void MainWindow::on_pushButton_6_clicked()
{
//    TODO: Load moving image
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("open a file."),
            "D:/",
            tr("All files(*.*)"));
    //ui->in_registration_1_moving_img_name->setText(fileName);
}

void MainWindow::on_pushButton_7_clicked()
{
//    TODO: Registration
    RegistrationParams registration_params;
   /* registration_params.fixed_img_name = ui->in_registration_0_fixed_img_name->text().toStdString();
    registration_params.moving_img_name = ui->in_registration_1_moving_img_name->text().toStdString();
    registration_params.registration_type = RegistrationType(ui->in_registration_4_regi_type->currentIndex());
    registration_params.metrics_type = MetricsType(ui->in_registration_5_metrics_type->currentIndex());*/
}

void MainWindow::on_pushButton_clicked()
{
//    TODO: Generated surface
    ProcessParams process_params;
    process_params.img_name = ui->in_gray_0_img->currentText().toStdString();
    process_params.isosurface_value = ui->in_gray_1_isosurface_value->value();
    process_params.surface_name = ui->in_gray_2_surface_name->text().toStdString();
}

void MainWindow::on_start_smoothing_button_clicked()
{
    SmoothingParams params;
    params.smooth_type = ui->smoothing_toolBox->currentIndex();
    switch (params.smooth_type) {
    case 0:
        params.kernel_size = ui->in_smooth_gaussian_0_kernel_size->value();
        params.sigma_x = ui->in_smooth_gaussian_1_sigma_x->value();
        break;
    case 1:
        params.kernel_size = ui->in_smooth_mean_0_kernel_size->value();
        break;
    case 2:
        params.kernel_size = ui->in_smooth_median_0_kernel_size->value();
        break;
    default:
        break;
    }
//    TODO
}

void MainWindow::on_detect_edge_button_clicked()
{
    EdgeDetectParams params;
    params.threshold_1 = ui->in_edge_0_thresh_1->value();
    params.threshold_2 = ui->in_edge_0_thresh_2->value();
//    TODO
}

void MainWindow::on_start_thresholding_button_clicked()
{
	ThresholdingParams params;
	params.lower_value = this->ui->in_lower_value->value();
	params.upper_value = this->ui->in_upper_value->value();

	vtkSmartPointer<vtkImageData> image_threshold_result;

	image_threshold(image_vtk_, image_threshold_result, params);

	//for (int i = 0; i < 3; i++)
	//{
	//	riw_[i]->SetSlice(this->dims[i] / 2);
	//	riw_[i]->SetInputData(image_threshold_result);
	//	riw_[i]->SetSliceOrientation(i);

	//	riw_[i]->Render();

	//	riw_[i]->GetRenderer()->ResetCamera();

	//}


}
