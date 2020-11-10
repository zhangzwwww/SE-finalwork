#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

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


#include "struct_define.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	image_(nullptr)
{
    ui->setupUi(this);
    init_VTKView();

    connect(ui->actionLoadImage, SIGNAL(triggered()), this, SLOT(on_loadImage_clicked()));
	connect(ui->actionVolume_Rendering, SIGNAL(triggered()), this, SLOT(on_Volume_clicked()));

	
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
		//Ŀǰ���޷�ʵ��ͬ�����´�����λ
	}

	//viewer�����Դ�renderwindow����û��interactor������ʹ��ui->QVTKWidget�Դ���interactor��ֵ��viewer
	//��ʼ��ʱ��û��Ϊÿ��riw���ñ�����ɫ��Ҳû������view1->show()�����ڳ�������ʱ���Զ�show������ͼ
	//this->ui->view1->SetRenderWindow(riw_[0]->GetRenderWindow());
	riw_[0]->SetRenderWindow(this->ui->view1->GetRenderWindow());
	riw_[0]->SetupInteractor(
		this->ui->view1->GetRenderWindow()->GetInteractor());

	riw_[1]->SetRenderWindow(this->ui->view2->GetRenderWindow());
	riw_[1]->SetupInteractor(
		this->ui->view2->GetRenderWindow()->GetInteractor());

	riw_[2]->SetRenderWindow(this->ui->view3->GetRenderWindow());
	riw_[2]->SetupInteractor(
		this->ui->view3->GetRenderWindow()->GetInteractor());

	//this->ui->view1->setVisible(true);


	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	ren->SetBackground(1, 1, 1);
	ren->SetBackground2(0.5, 0.5, 0.5);
	ren->SetGradientBackground(1);

	this->ui->view4->GetRenderWindow()->AddRenderer(ren);



}

void MainWindow::on_loadImage_clicked()
{
	//QString path = "D:/";
	QString fileName = QFileDialog::getExistingDirectory(this, QString(tr("Open DICOM Image")));
	//QString fileName = QFileDialog::getOpenFileName(this, QString(tr("Open DICOM Image")), path);

	if (fileName.isEmpty() == true)
		return;

	image_ = NULL;

	QByteArray ba = fileName.toLocal8Bit();
	const char* fileName_str = ba.data();

	vtkSmartPointer< vtkDICOMImageReader > reader =
		vtkSmartPointer< vtkDICOMImageReader >::New();
	reader->SetDirectoryName(fileName_str);
	reader->Update();
	image_ = reader->GetOutput();

	//����֮ǰ��renderer������������һ������ɫ������renderer
	//this->cleanReconstruction();

	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	ren->SetBackground(1, 1, 1);
	ren->SetBackground2(0.5, 0.5, 0.5);
	ren->SetGradientBackground(1);
	this->ui->view4->GetRenderWindow()->AddRenderer(ren);

	reader->GetOutput()->GetDimensions(this->dims);

	//�����Զ��庯����ʾ����ͼ
	this->showImage();
	//dataReady = true;
}


void MainWindow::showImage()
{
	//ui������interactor,RenderWindowû��interactor
	//vtkSmartPointer< vtkCamera > camera = vtkSmartPointer< vtkCamera >::New();
	//vtkSmartPointer<vtkFourViewerCallback> cbk =
	//	vtkSmartPointer<vtkFourViewerCallback>::New();

	double range[2];
	image_->GetScalarRange(range);

	//riw[0]->SetSliceOrientationToXY();
	//riw[1]->SetSliceOrientationToYZ();
	//riw[2]->SetSliceOrientationToXZ();

	for (int i = 0; i < 3; i++)
	{
		riw_[i]->SetSlice(this->dims[i] / 2);

		//riw[i]->GetRenderer()->SetActiveCamera(camera);

		riw_[i]->SetInputData(image_);
		riw_[i]->SetSliceOrientation(i);

		//ui->label_dimesion->setText(dimsChar);

		riw_[i]->SetColorWindow(range[1] - range[0]);
		riw_[i]->SetColorLevel((range[0] + range[1]) / 2.0);

		//cbk->view[i] = riw_[i];
		//riw_[i]->GetInteractorStyle()->AddObserver(vtkCommand::WindowLevelEvent, cbk);
		//�����¾�Ϊ�β������ã�
		//riw[i]->GetWindowLevel()->SetLookupTable(riw[0]->GetWindowLevel()->GetLookupTable());

		riw_[i]->Render();
		//Ĭ�ϴ���
		//riw[i]->GetRenderer()->EraseOn();
		riw_[i]->GetRenderer()->ResetCamera();   //��Ĭ�ϵ�render
		//riw[i]->UpdateDisplayExtent();
	}

	cout << "range:" << range[0] << "---" << range[1] << endl;

	//this->ui->view1->show();
	//this->ui->view2->show();
	//this->ui->view3->show();
}

void MainWindow::on_Volume_clicked() 
{
	vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper =
		vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	volumeMapper->SetInputData(image_);

	//���ù��߲�������
	volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance() / 4);
	//����ͼ���������
	volumeMapper->SetAutoAdjustSampleDistances(0);
	volumeMapper->SetImageSampleDistance(2);

	vtkSmartPointer<vtkVolumeProperty> volumeProperty =
		vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->ShadeOn();  //�򿪻��߹ر���Ӱ����
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

	volumeProperty->SetScalarOpacity(compositeOpacity); //���ò�͸���ȴ��亯��
	volumeProperty->SetColor(colorFun);



	vtkSmartPointer<vtkVolume> volume =
		vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	////����ÿ�ε�actor�����ᱻ����������һ��
	//vtkSmartPointer<vtkRenderer> ren = 
	//     this->ui->view4->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
	//ren->AddVolume(volume);


	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	ren->SetBackground(1, 1, 1);
	ren->SetBackground2(0.5, 0.5, 0.5);
	ren->SetGradientBackground(1);
	ren->AddVolume(volume);
	this->ui->view4->GetRenderWindow()->AddRenderer(ren);
	ren->ResetCamera();
	this->ui->view4->GetRenderWindow()->Render();

}


void MainWindow::on_pushButton_4_clicked()
{
    // TODO: start fusion
    FusionParams fusion_params;
    fusion_params.img0 = ui->in_fusion_0_img0->currentText().toStdString();
    fusion_params.img1 = ui->in_fusion_1_img1->currentText().toStdString();
    fusion_params.opacity = double(ui->in_fusion_2_opacity->value())/100.0;
    fusion_params.fused_img_name = ui->in_fusion_3_fused_img_name->text().toStdString();
}

void MainWindow::on_in_fusion_2_opacity_valueChanged(int value)
{
    ui->out_fusion_0_opacity->setText(QString::number(double(value)/100.0));
}

void MainWindow::on_pushButton_5_clicked()
{
//    TODO: Load fixed image
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("open a file."),
            "D:/",
            tr("All files(*.*)"));
    ui->in_registration_0_fixed_img_name->setText(fileName);
}

void MainWindow::on_pushButton_6_clicked()
{
//    TODO: Load moving image
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("open a file."),
            "D:/",
            tr("All files(*.*)"));
    ui->in_registration_1_moving_img_name->setText(fileName);
}

void MainWindow::on_pushButton_7_clicked()
{
//    TODO: Registration
    RegistrationParams registration_params;
    registration_params.fixed_img_name = ui->in_registration_0_fixed_img_name->text().toStdString();
    registration_params.moving_img_name = ui->in_registration_1_moving_img_name->text().toStdString();
    registration_params.registration_type = RegistrationType(ui->in_registration_4_regi_type->currentIndex());
    registration_params.metrics_type = MetricsType(ui->in_registration_5_metrics_type->currentIndex());
}

void MainWindow::on_pushButton_clicked()
{
//    TODO: Generated surface
    ProcessParams process_params;
    process_params.img_name = ui->in_gray_0_img->currentText().toStdString();
    process_params.isosurface_value = ui->in_gray_1_isosurface_value->value();
    process_params.surface_name = ui->in_gray_2_surface_name->text().toStdString();
}
