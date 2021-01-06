#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextCodec>

#include <vtkInteractorStyleRubberBand2D.h>
#include <vtkLineSource.h>
#include <vtkCoordinate.h>

class vtkPointPickerCallback : public vtkCommand
{
public:
    static vtkPointPickerCallback* New()
    {
        return new vtkPointPickerCallback;
    }
    vtkPointPickerCallback() {
        is_drawing = false;
        is_marking = true;
        cur_view = -1;
        for (int i = 0; i < 4; i++) {
            actors[i] = vtkSmartPointer<vtkActor>::New();
        }
    }

    void Execute(vtkObject* caller, unsigned long ev, 
        void* callData)
    {
        if (is_marking == false) {
            return;
        }
        if (!(ev == vtkCommand::RightButtonPressEvent || ev == vtkCommand::RightButtonReleaseEvent ||
             (ev == vtkCommand::MouseMoveEvent && is_drawing == true))) {
            return;
        }
        vtkInteractorStyleImage* style =
            dynamic_cast<vtkInteractorStyleImage*>(caller);
        int display_pos[2];

        if (style) {
            for (int i = 0; i < 3; i++) {
                if (style == this->view[i]->GetInteractorStyle()) {
                    cur_view = i;
                    cur_slice = this->view[i]->GetSlice();
                    this->interactor[i]->GetEventPosition(display_pos[0], display_pos[1]);
                    break;
                }
            }
        }
        if (ev == vtkCommand::RightButtonPressEvent)
        {
            if (vector_displaypos[cur_view].size() > 1)
            {
                return;
            }

            start_pos[0] = display_pos[0];
            start_pos[1] = display_pos[1];
            is_drawing = true;

            auto render = this->view[cur_view]->GetRenderer();

            interactor[cur_view]->GetPicker()->Pick(start_pos[0],
                start_pos[1],
                0,  // always zero.
                render);

            double picked[3];

            //interactor[0]->GetPicker()->GetPickPosition(picked);

            double start_position[3] = { double(start_pos[0]), double(start_pos[1]), 0.0 };

            render->SetDisplayPoint(start_position);
            render->DisplayToWorld();
            render->GetWorldPoint(picked);

            qDebug() << picked[0] << picked[1] << picked[2];

            
            //vector_picked[cur_view].push_back({picked[0], picked[1], picked[2]});
            vector_displaypos[cur_view].push_back({ start_pos[0], start_pos[1]});

            vtkSmartPointer<vtkSphereSource> sphereSource =
                vtkSmartPointer<vtkSphereSource>::New();
            sphereSource->Update();

            vtkSmartPointer<vtkPolyDataMapper> mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputConnection(sphereSource->GetOutputPort());
            vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
            actor->SetMapper(mapper);
            actor->SetPosition(picked);
            actor->SetScale(3);
            actor->SetDragable(true);
            actor->SetPickable(true);
            
            actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
            //this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);
            //this->Interactor->GetRenderWindow()->GetRenderers()->GetNextRenderer()->AddActor(actor);
            vtkActorCollection* actorCollection = render->GetActors();
            int num = actorCollection->GetNumberOfItems();
            actorCollection->InitTraversal();
            //vtkActor* actortemp = actorCollection->GetNextActor();
            for (int i = 0; i < num; ++i)
            {
                vtkActor* actortemp = actorCollection->GetNextActor();
                //render->RemoveActor(actortemp);
            }
            render->AddActor(actor);


            //for (int i = 0; i < 4; i++) {
            //    this->view[cur_view]->GetRenderer()->AddActor(actors[i]);
            //    //actors[i]->GetProperty()-
            //}
            qDebug()<<"cur view:"<<cur_view<<",cur index:"<<cur_slice<<
                                        ",start pos:("<<display_pos[0]<<","<<display_pos[1]<<")";
        } else if (ev == vtkCommand::MouseMoveEvent) {
            end_pos[0] = display_pos[0];
            end_pos[1] = display_pos[1];
            DrawRect();
            qDebug()<<"cur view:"<<cur_view<<",cur index:"<<cur_slice<<
                                        ",end pos:("<<display_pos[0]<<","<<display_pos[1]<<")";
        } else if (ev == vtkCommand::RightButtonReleaseEvent) {
            is_drawing = false;
            //for (int i = 0; i < 4; i++) {
            //    this->view[cur_view]->GetRenderer()->RemoveActor(actors[i]);
            //}

            qDebug() << "release:" <<  vector_picked[cur_view].size();

            auto render = this->view[cur_view]->GetRenderer();

            vtkActorCollection* actorCollection = render->GetActors();
            int num = actorCollection->GetNumberOfItems();
            actorCollection->InitTraversal();
            //vtkActor* actortemp = actorCollection->GetNextActor();
            if (vector_displaypos[cur_view].size() == 2)
            {
                /*auto vct_picked_1 = vector_picked[cur_view].at(0);
                double point1[3];
                point1[0] = vct_picked_1.at(0); point1[1] = vct_picked_1.at(1); point1[2] = vct_picked_1.at(2);

                auto vct_picked_2 = vector_picked[cur_view].at(1);
                double point2[3];
                point2[0] = vct_picked_2.at(0); point2[1] = vct_picked_2.at(1); point2[2] = vct_picked_2.at(2);*/

                auto start_vec = vector_displaypos[cur_view].at(0);
                double start[2];
                start[0] = start_vec.at(0); start[1] = start_vec.at(1);

                auto end_vec = vector_displaypos[cur_view].at(1);
                double end[2];
                end[0] = end_vec.at(0); end[1] = end_vec.at(1);


                double point1[2];
                double point2[2];
                double point3[2];
                double point4[2];

                double left[2];
                double right[2];

                left[0] = start[0] <= end[0] ? start[0] : end[0];
                left[1] = start[1] <= end[1] ? start[1] : end[1];

                right[0] = start[0] > end[0] ? start[0] : end[0];
                right[1] = start[1] > end[1] ? start[1] : end[1];

                point1[0] = left[0];  point1[1] = left[1];  //point1[2] = 0.0;
                point2[0] = left[0];  point2[1] = right[1]; //point2[2] = 0.0;
                point3[0] = right[0]; point3[1] = right[1]; //point3[2] = 0.0;
                point4[0] = right[0]; point4[1] = left[1];  //point4[2] = 0.0;

                //double p1_world[3]; 
                //double p2_world[3];
                //double p3_world[3];
                //double p4_world[3];

                qDebug() << "Right: " << right[0] << right[1];
                qDebug() << "Left:  " << left[0] << left[1];

                qDebug() << "point1: " << point1[0] << point1[1];
                qDebug() << "point2:  " << point2[0] << point2[1];
                qDebug() << "point3: " << point3[0] << point3[1];
                qDebug() << "point4:  " << point4[0] << point4[1];

                //for (int i = 0; i < 4; i++)

                vtkSmartPointer<vtkCoordinate> pCoorPress1 = vtkSmartPointer<vtkCoordinate>::New();
                pCoorPress1->SetCoordinateSystemToDisplay();
                pCoorPress1->SetValue(point1);
                double* p1_world = pCoorPress1->GetComputedWorldValue(render);

                vtkSmartPointer<vtkCoordinate> pCoorPress2 = vtkSmartPointer<vtkCoordinate>::New();
                pCoorPress2->SetCoordinateSystemToDisplay();
                pCoorPress2->SetValue(point2);
                double* p2_world = pCoorPress2->GetComputedWorldValue(render);

                vtkSmartPointer<vtkCoordinate> pCoorPress3 = vtkSmartPointer<vtkCoordinate>::New();
                pCoorPress3->SetCoordinateSystemToDisplay();
                pCoorPress3 ->SetValue(point3);
                double* p3_world = pCoorPress3->GetComputedWorldValue(render);


                vtkSmartPointer<vtkCoordinate> pCoorPress4 = vtkSmartPointer<vtkCoordinate>::New();
                pCoorPress4->SetCoordinateSystemToDisplay();
                pCoorPress4->SetValue(point4);
                double* p4_world = pCoorPress4->GetComputedWorldValue(render);

                //GetScreentPos(point1, p1_world, cur_view);

                //GetScreentPos(point2, p2_world, cur_view);

                //render->SetDisplayPoint(point1);
                //render->DisplayToWorld();
                //render->GetWorldPoint(p1_world);


                //render->SetDisplayPoint(point2);
                //render->DisplayToWorld();
                //render->GetWorldPoint(p2_world);
                    
                //render->SetDisplayPoint(point3);
                //render->DisplayToWorld();
                //render->GetWorldPoint(p3_world);

                //render->SetDisplayPoint(point4);
                //render->DisplayToWorld();
                //render->GetWorldPoint(p4_world);


                qDebug() << p1_world[0] << p1_world[1] << p1_world[2];
                qDebug() << p2_world[0] << p2_world[1] << p2_world[2];
                qDebug() << p3_world[0] << p3_world[1] << p3_world[2];
                qDebug() << p4_world[0] << p4_world[1] << p4_world[2];

                

                auto actor1 = this->SetLine(p1_world, p2_world);
                auto actor2 = this->SetLine(p2_world, p3_world);
                auto actor3 = this->SetLine(p3_world, p4_world);
                auto actor4 = this->SetLine(p4_world, p1_world);

                if (actor1 == nullptr)
                {
                    return;
                }

                /*vtkSmartPointer<vtkLineSource> line_source = vtkSmartPointer<vtkLineSource>::New();
                line_source->SetPoint1(point1);
                line_source->SetPoint2(point2);
                line_source->Update();

                vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(line_source->GetOutputPort());
                
                vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
                actor->SetMapper(mapper);
                actor->GetProperty()->SetLineWidth(2);
                actor->GetProperty()->SetColor(0.0, 1.0, 0.0);*/


                render->AddActor(actor1);
                render->AddActor(actor2);
                render->AddActor(actor3);
                render->AddActor(actor4);
            }

            
            this->view[cur_view]->Render();
        }
        return;
    }

    vtkSmartPointer<vtkActor> SetLine(double start_point[], double end_point[]) 
    {
        if (cur_view < 0 || cur_view > 2) {
            return nullptr;
        }

        vtkSmartPointer<vtkLineSource> line_source = vtkSmartPointer<vtkLineSource>::New();
        line_source->SetPoint1(start_point);
        line_source->SetPoint2(end_point);
        line_source->Update();

        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(line_source->GetOutputPort());
        
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->SetLineWidth(2);
        actor->GetProperty()->SetColor(0.0,1.0,0.0);

        return actor;
    }

    void DrawRect() {
        //if (cur_view < 0 || cur_view > 2) {
        //    return;
        //}
        //double start_position[3] = {double(start_pos[0]), double(start_pos[1]), 0.0};
        //double end_position[3] = {double(end_pos[0]), double(end_pos[1]), 0.0};

        //double start[3];
        //double end[3];
        //GetScreentPos(start_position, start, cur_view);
        //GetScreentPos(end_position, end, cur_view);
        //qDebug() << "start pos:"<< start[0] << start[1] << start[2]<<",end pos:"<<end[0]<<end[1]<<end[2];

        //double point1[3];
        //double point2[3];
        //double point3[3];
        //double point4[3];

        //double left[2];
        //double right[2];

        //left[0] = start[0]<=end[0] ? start[0] : end[0];
        //left[1] = start[1]<=end[1] ? start[1] : end[1];

        //right[0] = start[0]>end[0] ? start[0] : end[0];
        //right[1] = start[1]>end[1] ? start[1] : end[1];

        //point1[0] = left[0];  point1[1] = left[1];  point1[2] = 0;
        //point2[0] = left[0];  point2[1] = right[1]; point2[2] = 0;
        //point3[0] = right[0]; point3[1] = right[1]; point3[2] = 0;
        //point4[0] = right[0]; point4[1] = left[1];  point4[2] = 0;

        //this->SetLine(point1,point2, actors[0]);
        //this->SetLine(point2,point3, actors[1]);
        //this->SetLine(point3,point4, actors[2]);
        //this->SetLine(point4,point1, actors[3]);

        //this->view[cur_view]->Render();
    }

    void StartMark() {
        is_marking = true;
    }

    void EndMark() {
        is_marking = false;
    }

    void GetScreentPos(double displayPos[2], double world[2], int current_view)
    {
      vtkSmartPointer<vtkRenderer> renderer = this->view[current_view]->GetRenderer();
        renderer->SetDisplayPoint(displayPos);
        renderer->DisplayToWorld();
        renderer->GetWorldPoint(world);

        //interactor[current_view]->GetPicker()->Pick(displayPos[0],
        //    displayPos[1],
        //    0,  // always zero.
        //    renderer);
        //interactor[current_view]->GetPicker()->GetPickPosition(world);

    }


    vtkImageViewer2* view[3];
    vtkRenderWindowInteractor* interactor[3];
    vtkRenderer* render[3];
    vtkSmartPointer<vtkActor> actors[4];

    vector<vector<double>> vector_picked[3];
    vector<vector<int>> vector_displaypos[3];
    int cur_view;
    int cur_slice;
    int start_pos[2];
    int end_pos[2];
    bool is_drawing;
    bool is_marking;
};



/*--------------------- Definition for subclass ------------------*/
MainWindow::vtkSharedWindowLevelCallback* MainWindow::vtkSharedWindowLevelCallback::New(){
    return new vtkSharedWindowLevelCallback;
}

void MainWindow::vtkSharedWindowLevelCallback::Execute(vtkObject *caller, unsigned long ev, void *callData){
    if (ev == vtkCommand::WindowLevelEvent)
    {
        vtkInteractorStyleImage* style =
            dynamic_cast<vtkInteractorStyleImage*>(caller);

        if (style)
        {
            if (style == this->view[0]->GetInteractorStyle())
            {
                view[1]->SetColorLevel(view[0]->GetColorLevel());
                view[1]->SetColorWindow(view[0]->GetColorWindow());
                view[2]->SetColorLevel(view[0]->GetColorLevel());
                view[2]->SetColorWindow(view[0]->GetColorWindow());
            }
            else if (style == this->view[1]->GetInteractorStyle())
            {
                view[0]->SetColorLevel(view[1]->GetColorLevel());
                view[0]->SetColorWindow(view[1]->GetColorWindow());
                view[2]->SetColorLevel(view[1]->GetColorLevel());
                view[2]->SetColorWindow(view[1]->GetColorWindow());
            }
            else if (style == this->view[2]->GetInteractorStyle())
            {
                view[0]->SetColorLevel(view[2]->GetColorLevel());
                view[0]->SetColorWindow(view[2]->GetColorWindow());
                view[1]->SetColorLevel(view[2]->GetColorLevel());
                view[1]->SetColorWindow(view[2]->GetColorWindow());
            }
        }

        for (int i = 0; i < 3; i++)
        {
            this->view[i]->Render();
        }

        return;
    }
}

MainWindow::vtkSharedWindowLevelCallback::vtkSharedWindowLevelCallback(){

}


/*--------------------- End definition for subclass --------------*/



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->data_manager->clear();
//    ui->patientSelector->setFocus();
//    TODO: getAllPatient and update ui, suppose we input a vector<QString> vec, use ui->patientSelector->addItem(vec[i]) to update

    //const char* path = ":/qssfile/dark_theme.qss";
    //QFile qssfile(path);
    //qssfile.open(QFile::ReadOnly);
    //QString qss;
    //qss = qssfile.readAll();
    //this->setStyleSheet(qss);
    this->init_views();

    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->mainToolBar->setFixedHeight(50);
    ui->registration_progressBar->setVisible(false);
    ui->mesher_progressBar->setVisible(false);

    connect(ui->action_open_file, SIGNAL(triggered()), this, SLOT(load_image()));
    connect(ui->action_visualization, SIGNAL(triggered(bool)), this, SLOT(volume_rendering(bool)));

    connect(ui->zoomBtn1, SIGNAL(clicked()), this, SLOT(view_zoom_to_fit()));
    connect(ui->zoomBtn2, SIGNAL(clicked()), this, SLOT(view_zoom_to_fit()));
    connect(ui->zoomBtn3, SIGNAL(clicked()), this, SLOT(view_zoom_to_fit()));
    connect(ui->fullScreenBtn1, SIGNAL(clicked(bool)), this, SLOT(view_full_screen(bool)));
    connect(ui->fullScreenBtn2, SIGNAL(clicked(bool)), this, SLOT(view_full_screen(bool)));
    connect(ui->fullScreenBtn3, SIGNAL(clicked(bool)), this, SLOT(view_full_screen(bool)));
    connect(ui->fullScreenBtn4, SIGNAL(clicked(bool)), this, SLOT(view_full_screen(bool)));
    connect(ui->ScrollBar1, SIGNAL(valueChanged(int)), this, SLOT(view_change_slice()));
    connect(ui->ScrollBar2, SIGNAL(valueChanged(int)), this, SLOT(view_change_slice()));
    connect(ui->ScrollBar3, SIGNAL(valueChanged(int)), this, SLOT(view_change_slice()));

   
    connect(ui->start_registration_btn, SIGNAL(clicked()), this, SLOT(start_registration()));
    connect(ui->startFusionButton, SIGNAL(clicked()), this, SLOT(start_fusion()));
    connect(ui->voxel2meshBtn, SIGNAL(clicked()), this, SLOT(generate_surface()));
    connect(ui->opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(slidervalueChanged(int)));
    connect(ui->start_segmentation_btn, SIGNAL(clicked()), this, SLOT(start_segmentation()));

    connect(ui->clean_actors_btn, SIGNAL(clicked()), this, SLOT(clean_actors()));
    connect(ui->clear_manager_btn, SIGNAL(clicked()), this, SLOT(clear_manager()));

}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::init_views()
{

    for (int i = 0; i < 3; i++)
    {
        riw_[i] = vtkSmartPointer< vtkImageViewer2 >::New();
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

//    this->ui->view4->GetRenderWindow()->AddRenderer(renderer3D_);


}

void MainWindow::load_image()
{
    QString fileName = QFileDialog::getExistingDirectory(this, QString(tr("Open DICOM Image")));

    if (fileName.isEmpty() == true)
        return;

	QTextCodec* code = QTextCodec::codecForName("GB2312");
	std::string name = code->fromUnicode(fileName).data();

	//QByteArray ba = fileName.toLocal8Bit();
	//const char* fileName_str = ba.data();

	RegistrationWorker worker;
    itk::Image<float, 3>::Pointer image_itk = worker.readImageDICOM(name.c_str());

    if (image_itk == nullptr)
    {
        QMessageBox::warning(nullptr,
            tr("Read Image Error"),
            tr("Read image error, Please read the correct Image."),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

        return;
    }

    using FilterType = itk::ImageToVTKImageFilter<itk::Image<float, 3>>;
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(image_itk);

    try
    {
        filter->Update();
    }
    catch (itk::ExceptionObject & error)
    {
        QMessageBox::warning(nullptr,
            tr("Read Image Error"),
            tr(error.GetDescription()),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        return;
    }

    vtkSmartPointer<vtkImageData> image_vtk = filter->GetOutput();

    itk_image_collection_.push_back(image_itk);
    vtk_image_collection_.push_back(image_vtk);

    ImageDataItem image_item;
    image_item.image_path = fileName;
    image_item.image_name = GetFileName(fileName);
    image_item.image_data = image_vtk;

    image_tree_.push_back(vector<ImageDataItem>{image_item});

    this->update_data_manager();

    // clean the current volume
    this->clean_view4();
    this->show_image();
}


void MainWindow::show_image()
{
    vtkSmartPointer< vtkSharedWindowLevelCallback > sharedWLcbk =
        vtkSmartPointer< vtkSharedWindowLevelCallback >::New();

    vtkSmartPointer< vtkPointPickerCallback > point_picker_cbk =
        vtkSmartPointer< vtkPointPickerCallback >::New();

    point_picker_cbk->interactor[0] = this->ui->view1->GetInteractor();
    point_picker_cbk->interactor[1] = this->ui->view2->GetInteractor();
    point_picker_cbk->interactor[2] = this->ui->view3->GetInteractor();


    if (cur_selected_image_ind_[0] == -1)
    {
        this->ui->view1->hide();
        this->ui->view2->hide();
        this->ui->view3->hide();
        return;
    }

    vtkSmartPointer<vtkImageData> current_selected_image = image_tree_[cur_selected_image_ind_[0]][cur_selected_image_ind_[1]].image_data;
    
    if (current_selected_image == nullptr)
    {
        QMessageBox::warning(nullptr,
            tr("Error"),
            tr("Image is NULL."),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        return;
    }


    double range[2];
    int dims[3];
    current_selected_image->GetScalarRange(range);
    current_selected_image->GetDimensions(dims);

    //vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper =
    //	vtkSmartPointer<vtkImageSliceMapper>::New();
    //imageSliceMapper->SetInputData(image_vtk_);
    //vtkSmartPointer<vtkImageSlice> imageSlice = vtkSmartPointer<vtkImageSlice>::New();
    //imageSlice->SetMapper(imageSliceMapper);
    //for (int crntViewLabel = 0; crntViewLabel < 3; crntViewLabel++)
    //{
    //	m_ImageStack2D[crntViewLabel]->AddImage(imageSlice);
    //}

    this->ui->view1->show();
    this->ui->view2->show();
    this->ui->view3->show();


    for (int i = 0; i < 3; i++)
    {
        riw_[i]->SetInputData(current_selected_image);

        riw_[i]->SetSliceOrientation(i);
        riw_[i]->SetSlice(dims[i] / 2);      
        //riw_[i]->SetColorWindow((range[1] - range[0]));
        //riw_[i]->SetColorLevel((range[0] + range[1]) / 2.0);

        sharedWLcbk->view[i] = riw_[i];
        point_picker_cbk->view[i] = riw_[i];

        riw_[i]->GetInteractorStyle()->AddObserver(vtkCommand::WindowLevelEvent, sharedWLcbk);
        riw_[i]->GetInteractorStyle()->AddObserver(vtkCommand::RightButtonPressEvent, point_picker_cbk);
        riw_[i]->GetInteractorStyle()->AddObserver(vtkCommand::RightButtonReleaseEvent, point_picker_cbk);
        //riw_[i]->GetInteractorStyle()->AddObserver(vtkCommand::MouseMoveEvent, point_picker_cbk);

    
    }

    this->ui->ScrollBar1->setMaximum(dims[0]);
    this->ui->ScrollBar2->setMaximum(dims[1]);
    this->ui->ScrollBar3->setMaximum(dims[2]);
    this->ui->ScrollBar1->setSliderPosition(dims[0] / 2 - 1);
    this->ui->ScrollBar2->setSliderPosition(dims[1] / 2 - 1);
    this->ui->ScrollBar3->setSliderPosition(dims[2] / 2 - 1);
    
    this->view_zoom_to_fit();
}

void MainWindow::volume_rendering(bool status)
{
	if (status == false)
	{
		if (volume_ != nullptr)
		{
			renderer3D_->RemoveVolume(volume_);
			this->ui->view4->GetRenderWindow()->Render();
		}

		return;
	}
	else {
        qDebug() << cur_selected_image_ind_[0] << cur_selected_image_ind_[1];

        if (cur_selected_image_ind_[0] == -1)
        {
            QMessageBox::warning(nullptr,
                tr("Error"),
                tr("Please select image"),
                QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

            ui->action_visualization->setChecked(false);
            return;
        }

        vtkSmartPointer<vtkImageData> current_selected_image_ = 
            image_tree_[cur_selected_image_ind_[0]][cur_selected_image_ind_[1]].image_data;


		if (current_selected_image_ == nullptr)
		{
			QMessageBox::warning(nullptr,
				tr("Error"),
				tr("Image NULL."),
				QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		
			ui->action_visualization->setChecked(false);
			return;
		}

		if (volume_ == nullptr)
		{
			volume_ = vtkSmartPointer<vtkVolume>::New();
		}

		ui->action_visualization->setEnabled(0);

		vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper =
			vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
		volumeMapper->SetInputData(current_selected_image_);

		volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance() / 2);
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

        // TODO: add slider to control the opacity
		compositeOpacity->AddPoint(-3024, 0, 0.5, 0.0);
		compositeOpacity->AddPoint(-16, 0, .49, .61);
		compositeOpacity->AddPoint(641, .72, .5, 0.0);
		compositeOpacity->AddPoint(3071, .71, 0.5, 0.0);

        //compositeOpacity->AddPoint(70, 0);
        //compositeOpacity->AddPoint(90, 0.4);
        //compositeOpacity->AddPoint(180, 0.6);

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

		ui->action_visualization->setEnabled(1);
	}
}


void MainWindow::clean_view4()
{
    this->ui->view4->GetRenderWindow()->RemoveRenderer(renderer3D_);

    renderer3D_ = vtkSmartPointer<vtkRenderer>::New();
    renderer3D_->SetBackground(1, 1, 1);
    renderer3D_->SetBackground2(0.5, 0.5, 0.5);
    renderer3D_->SetGradientBackground(1);

    this->ui->view4->GetRenderWindow()->AddRenderer(renderer3D_);
    this->ui->view4->GetRenderWindow()->Render();
}

void MainWindow::view_zoom_to_fit()
{
    for (int i = 0; i < 3; i++)
    {
        riw_[i]->GetRenderer()->ResetCamera();
        riw_[i]->Render();
    }
}

void MainWindow::view_full_screen(bool full_status)
{
    QObject* obj = sender();

    if (obj == this->ui->fullScreenBtn1)
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
    if (obj == this->ui->fullScreenBtn2)
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
    if (obj == this->ui->fullScreenBtn3)
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
    if (obj == this->ui->fullScreenBtn4)
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
}


void MainWindow::view_change_slice()
{
    QObject* obj = sender();
    if (obj == this->ui->ScrollBar1)
    {
        QScrollBar* tempScroll = qobject_cast<QScrollBar*>(obj);
        riw_[0]->SetSlice(tempScroll->value());
        riw_[0]->Render();
    }
    if (obj == this->ui->ScrollBar2)
    {
        QScrollBar* tempScroll = qobject_cast<QScrollBar*>(obj);
        riw_[1]->SetSlice(tempScroll->value());
        riw_[0]->Render();
    }
    if (obj == this->ui->ScrollBar3)
    {
        QScrollBar* tempScroll = qobject_cast<QScrollBar*>(obj);
        riw_[2]->SetSlice(tempScroll->value());
        riw_[0]->Render();
    }
}


void MainWindow::clean_actors()
{
    clean_view4();
    ui->action_visualization->setChecked(false);
}



// Algorithm Function: 

// Registration
void MainWindow::start_registration()
{
    if (ui->FixedImageSelector->count() == 0)
    {
        setMandatoryField(ui->FixedImageSelector, true);
        QMessageBox::warning(nullptr,
            tr("Error"),
            tr("No Image Selected."),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        return;
    }

    if (ui->MovingImageSelector->count() == 0)
    {
        setMandatoryField(ui->MovingImageSelector, true);
        QMessageBox::warning(nullptr,
            tr("Error"),
            tr("No Image Selected."),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        return;
    }

    RegistrationWorker::FixedImageType::Pointer fixed_image_itk;
    RegistrationWorker::FixedImageType::Pointer moving_image_itk;

    // loop for finding corresponding ImageDataItem
    QString fixed_name = ui->FixedImageSelector->currentText();
    QString moving_name = ui->MovingImageSelector->currentText();

    for (vector<ImageDataItem>& vec : image_tree_) {
        for (ImageDataItem& item : vec){
            if (fixed_name == item.image_name){
                if (item.image_data == nullptr){
                    setMandatoryField(ui->FixedImageSelector, true);
                    QMessageBox::warning(nullptr,
                        tr("Error"),
                        tr("No Image Selected."),
                        QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                    return;
                }
                else {
                    setMandatoryField(ui->FixedImageSelector, false);

                    using VTK2ITKType = itk::VTKImageToImageFilter<RegistrationWorker::FixedImageType>;
                    VTK2ITKType::Pointer vtk2itk_filter = VTK2ITKType::New();
                    vtk2itk_filter->SetInput(item.image_data);

                    try
                    {
                        vtk2itk_filter->Update();
                    }
                    catch (itk::ExceptionObject& error)
                    {
                        QMessageBox::warning(nullptr,
                            tr("Error"),
                            tr(error.GetDescription()),
                            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                        return;
                    }

                    fixed_image_itk = vtk2itk_filter->GetOutput();
                }
            }
        }
    }

    for (vector<ImageDataItem>& vec : image_tree_) {
        for (ImageDataItem& item : vec) {
            if (moving_name == item.image_name) {
                if (item.image_data == nullptr) {
                    setMandatoryField(ui->MovingImageSelector, true);
                    QMessageBox::warning(nullptr,
                        tr("Error"),
                        tr("No Image Selected."),
                        QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                    return;
                }
                else {
                    setMandatoryField(ui->MovingImageSelector, false);

                    using VTK2ITKType = itk::VTKImageToImageFilter<RegistrationWorker::FixedImageType>;
                    VTK2ITKType::Pointer vtk2itk_filter = VTK2ITKType::New();
                    vtk2itk_filter->SetInput(item.image_data);

                    try
                    {
                        vtk2itk_filter->Update();
                    }
                    catch (itk::ExceptionObject& error)
                    {
                        QMessageBox::warning(nullptr,
                            tr("Error"),
                            tr(error.GetDescription()),
                            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                        return;
                    }

                    moving_image_itk = vtk2itk_filter->GetOutput();
                }
            }
        }
    }


    ui->start_registration_btn->setVisible(false);
    ui->registration_progressBar->setVisible(true);

    RegistrationWorker worker;
    worker.setFixedImage(fixed_image_itk);
    worker.setMovingImage(moving_image_itk);
    worker.setIteration(ui->paramIterationSpinBox->value());
    worker.setMaximumStepLength(ui->paramMaxSpinBox->value());
    worker.setMinimumStepLength(ui->paramMinSpinBox->value());
    worker.setRelaxationFactor(ui->paramRelaxSpinBox->value());
    worker.setNumberOfSamples(ui->paramNumberspinBox->value());
    worker.setDefualtPixelValue(ui->paramDPVSpinBox->value());

    qRegisterMetaType<itk::DataObject::Pointer>("itk::DataObject::Pointer");

    QObject::connect(&worker, SIGNAL(finished(itk::DataObject::Pointer)),
        this, SLOT(workerIsDone(itk::DataObject::Pointer)));
    QObject::connect(&worker, SIGNAL(progress(float)), this, SLOT(workerProgressUpdate(float)));

    worker.process();

}

void MainWindow::workerProgressUpdate(float progress) 
{
    int progressInt = (int)(progress * 100.0f);
    ui->registration_progressBar->setValue(progressInt);
   
}

void MainWindow::workerIsDone(itk::DataObject::Pointer data) 
{
    RegistrationWorker::FixedImageType* result_image_itk =
        dynamic_cast<RegistrationWorker::FixedImageType*>(data.GetPointer());

    // cast itk image to vtk image
    using FilterType = itk::ImageToVTKImageFilter<RegistrationWorker::FixedImageType>;
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(result_image_itk);

    try
    {
        filter->Update();
    }
    catch (itk::ExceptionObject& error)
    {
        QMessageBox::warning(nullptr,
            tr("Error"),
            tr(error.GetDescription()),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        return;
    }

    vtkSmartPointer<vtkImageData> result_image_vtk = filter->GetOutput();

    vtk_image_collection_.push_back(result_image_vtk);
    itk_image_collection_.push_back(result_image_itk);

    QString item_name = ui->RegNameLineEdit->text();
    for (vector<ImageDataItem>& vec : image_tree_) {
        for (ImageDataItem& item : vec) {
            if (item_name == item.image_name) {
                item_name = "(1)" + item_name;
            }
        }
    }

    ImageDataItem image_item;
    image_item.image_name = QString(item_name);
    image_item.image_data = result_image_vtk;

    image_tree_.push_back(vector<ImageDataItem>{image_item});

    this->update_data_manager();

    ui->start_registration_btn->setVisible(true);
    ui->registration_progressBar->setVisible(false);
}

void MainWindow::start_fusion()
{
    if (ui->BlendImage0Selector->count() == 0)
    {
        setMandatoryField(ui->BlendImage0Selector, true);
        QMessageBox::warning(nullptr,
            tr("Error"),
            tr("No Image Selected."),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        return;
    }

    if (ui->BlendImage1Selector->count() == 0)
    {
        setMandatoryField(ui->BlendImage1Selector, true);
        QMessageBox::warning(nullptr,
            tr("Error"),
            tr("No Image Selected."),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        return;
    }

    RegistrationWorker::FixedImageType::Pointer image0Itk;
    RegistrationWorker::FixedImageType::Pointer image1Itk;

    // loop for finding corresponding ImageDataItem
    QString image0_name = ui->BlendImage0Selector->currentText();
    QString image1_name = ui->BlendImage1Selector->currentText();

    for (vector<ImageDataItem>& vec : image_tree_) {
        for (ImageDataItem& item : vec) {
            if (image0_name == item.image_name) {
                if (item.image_data == nullptr) {
                    setMandatoryField(ui->BlendImage0Selector, true);
                    QMessageBox::warning(nullptr,
                        tr("Error"),
                        tr("No Image Selected."),
                        QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                    return;
                }
                else {
                    setMandatoryField(ui->BlendImage0Selector, false);

                    using VTK2ITKType = itk::VTKImageToImageFilter<RegistrationWorker::FixedImageType>;
                    VTK2ITKType::Pointer vtk2itk_filter = VTK2ITKType::New();
                    vtk2itk_filter->SetInput(item.image_data);

                    try
                    {
                        vtk2itk_filter->Update();
                    }
                    catch (itk::ExceptionObject& error)
                    {
                        QMessageBox::warning(nullptr,
                            tr("Error"),
                            tr(error.GetDescription()),
                            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                        return;
                    }

                    image0Itk = vtk2itk_filter->GetOutput();
                }
            }
        }
    }

    for (vector<ImageDataItem>& vec : image_tree_) {
        for (ImageDataItem& item : vec) {
            if (image1_name == item.image_name) {
                if (item.image_data == nullptr) {
                    setMandatoryField(ui->BlendImage1Selector, true);
                    QMessageBox::warning(nullptr,
                        tr("Error"),
                        tr("No Image Selected."),
                        QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                    return;
                }
                else {
                    setMandatoryField(ui->BlendImage1Selector, false);

                    using VTK2ITKType = itk::VTKImageToImageFilter<RegistrationWorker::FixedImageType>;
                    VTK2ITKType::Pointer vtk2itk_filter = VTK2ITKType::New();
                    vtk2itk_filter->SetInput(item.image_data);

                    try
                    {
                        vtk2itk_filter->Update();
                    }
                    catch (itk::ExceptionObject& error)
                    {
                        QMessageBox::warning(nullptr,
                            tr("Error"),
                            tr(error.GetDescription()),
                            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                        return;
                    }

                    image1Itk = vtk2itk_filter->GetOutput();
                }
            }
        }
    }

    // 判断图像尺寸是否一致

    // isValidFusionSelection(itk 0 itk1)
    if (true) {

        // fusion
        int pos = ui->opacitySlider->value();
        double factor0 = static_cast<double>(pos / 10.0);
        double factor1 = 1 - factor0;

        using FilterType = itk::MultiplyImageFilter<RegistrationWorker::FixedImageType,
            RegistrationWorker::FixedImageType, RegistrationWorker::FixedImageType>;
        FilterType::Pointer filter0 = FilterType::New();
        filter0->SetInput(image0Itk);
        filter0->SetConstant(factor0);

        FilterType::Pointer filter1 = FilterType::New();
        filter1->SetInput(image1Itk);
        filter1->SetConstant(factor1);

        using AddImageFilterType = itk::AddImageFilter<itk::Image<float, 3>,
            itk::Image<float, 3>>;
        AddImageFilterType::Pointer addFilter = AddImageFilterType::New();
        addFilter->SetInput1(filter0->GetOutput());
        addFilter->SetInput2(filter1->GetOutput());
        addFilter->Update();

        // cast itk image to vtk image
        using toVTK_FilterType = itk::ImageToVTKImageFilter<itk::Image<float, 3>>;
        toVTK_FilterType::Pointer toVTK_filter = toVTK_FilterType::New();
        toVTK_filter->SetInput(addFilter->GetOutput());

        try
        {
            toVTK_filter->Update();
        }
        catch (itk::ExceptionObject& error)
        {
            QMessageBox::warning(nullptr,
                tr("Error"),
                tr(error.GetDescription()),
                QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
            return;
        }

        vtkSmartPointer<vtkImageData> result_image_vtk = toVTK_filter->GetOutput();

        vtk_image_collection_.push_back(result_image_vtk);
        itk_image_collection_.push_back(addFilter->GetOutput());

        QString item_name = ui->FusedNameLineEdit->text() + "-" + QString::number(factor0);
        for (vector<ImageDataItem>& vec : image_tree_) {
            for (ImageDataItem& item : vec) {
                if (item_name == item.image_name) {
                    item_name = "(1)" + item_name;
                }
            }
        }

        ImageDataItem image_item;
        image_item.image_name = QString(item_name);
        image_item.image_data = result_image_vtk;

        image_tree_.push_back(vector<ImageDataItem>{image_item});

        this->update_data_manager();
    }
}



void MainWindow::slidervalueChanged(int pos) {
    double factor0 = static_cast<double>(pos / 10.0);
    double factor1 = 1 - factor0;
    ui->opacity0label->setText("Image 0:  " + QString::number(factor0) + "  ");
    ui->opacity1label->setText("  Image 1:  " + QString::number(factor1));
}

void MainWindow::start_segmentation()
{
    if (ui->seg_image_selector->count() == 0)
    {
        setMandatoryField(ui->seg_image_selector, true);

        QMessageBox::warning(nullptr,
            tr("Error"),
            tr("No Image Selected."),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        return;
    }

    SegmentationWorker segmentation_worker;

    QString current_name = ui->seg_image_selector->currentText();

    for (vector<ImageDataItem>& vec : image_tree_)
    {
        for (ImageDataItem& item : vec)
        {
            if (current_name == item.image_name)
            {
                if (item.image_data == nullptr)
                {
                    setMandatoryField(ui->seg_image_selector, true);

                    QMessageBox::warning(nullptr,
                        tr("Error"),
                        tr("No Image Selected."),
                        QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                    return;

                }
                else {
                    setMandatoryField(ui->seg_image_selector, false);

                    ui->start_segmentation_btn->setVisible(false);
                    ui->segmentation_progressBar->setVisible(true);
                    ui->segmentation_progressBar->setValue(25);

                    using VTK2ITKType = itk::VTKImageToImageFilter<RegistrationWorker::FixedImageType>;
                    VTK2ITKType::Pointer vtk2itk_filter = VTK2ITKType::New();
                    vtk2itk_filter->SetInput(item.image_data);

                    try
                    {
                        vtk2itk_filter->Update();
                    }
                    catch (itk::ExceptionObject& error)
                    {
                        QMessageBox::warning(nullptr,
                            tr("Error"),
                            tr(error.GetDescription()),
                            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                        return;
                    }

                    // TODO
                    // 获取当前seed的坐标


                    SegmentationWorker::IndexType seed1;
                    seed1[0] = 369;
                    seed1[1] = 317;
                    seed1[2] = 93;

                    SegmentationWorker::IndexType seed2;
                    seed2[0] = 403;
                    seed2[1] = 236;
                    seed2[2] = 78;

                    SegmentationWorker::IndexType seed3;
                    seed3[0] = 404;
                    seed3[1] = 236;
                    seed3[2] = 66;

                    SegmentationWorker::IndexType seed4;
                    seed4[0] = 125;
                    seed4[1] = 236;
                    seed4[2] = 71;

                    SegmentationWorker::IndexType seed5;
                    seed5[0] = 128;
                    seed5[1] = 268;
                    seed5[2] = 69;

                    SegmentationWorker::IndexType seed6;
                    seed6[0] = 146;
                    seed6[1] = 264;
                    seed6[2] = 93;


                    std::vector<SegmentationWorker::IndexType> vecseed{ seed1, seed2, seed3, seed4, seed5, seed6 };

                    segmentation_worker.set_lower_value(ui->lower_value_seg->value());
                    segmentation_worker.set_upper_value(ui->upper_value_seg->value());

                    segmentation_worker.set_input_image(vtk2itk_filter->GetOutput());
                    segmentation_worker.set_seeds(vecseed);

                    ui->segmentation_progressBar->setValue(50);

                    segmentation_worker.update();
                    auto result_image_itk = segmentation_worker.get_output_image();
                    ui->segmentation_progressBar->setValue(75);

                    if (result_image_itk != nullptr)
                    {
                        // cast itk image to vtk image
                        using FilterType = itk::ImageToVTKImageFilter<RegistrationWorker::FixedImageType>;
                        FilterType::Pointer filter = FilterType::New();
                        filter->SetInput(result_image_itk);

                        try
                        {
                            filter->Update();
                        }
                        catch (itk::ExceptionObject& error)
                        {
                            QMessageBox::warning(nullptr,
                                tr("Error"),
                                tr(error.GetDescription()),
                                QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                            return;
                        }

                        vtkSmartPointer<vtkImageData> result_image_vtk = filter->GetOutput();

                        vtk_image_collection_.push_back(result_image_vtk);
                        itk_image_collection_.push_back(result_image_itk);

                        QString item_name = ui->seg_result_name->text() + "_" + current_name;
                        for (vector<ImageDataItem>& vec : image_tree_) {
                            for (ImageDataItem& item : vec) {
                                if (item_name == item.image_name) {
                                    item_name = "(1)" + item_name;
                                }
                            }
                        }
                        ImageDataItem image_item;
                        image_item.image_name = QString(item_name);
                        image_item.image_data = result_image_vtk;

                        vec.push_back(image_item);

                        this->update_data_manager();

                    }

                    ui->start_segmentation_btn->setVisible(true);
                    ui->segmentation_progressBar->setVisible(false);

                    break;
                }
            }
        }
    }


}


void MainWindow::generate_surface()
{
    if (ui->greyScaleImageSelector->count() == 0)
    {
        setMandatoryField(ui->greyScaleImageSelector, true);

        QMessageBox::warning(nullptr,
            tr("Error"),
            tr("No Image Selected."),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        return;
    }

    Voxel2Mesh voxel2mesh_filter;

    // loop for finding corresponding ImageDataItem
    QString current_name = ui->greyScaleImageSelector->currentText();
    
    for (vector<ImageDataItem>& vec : image_tree_)
    {
        for (ImageDataItem& item : vec)
        {
            if (current_name == item.image_name)
            {
                if (item.image_data == nullptr)
                {
                    setMandatoryField(ui->greyScaleImageSelector, true);

                    QMessageBox::warning(nullptr,
                        tr("Error"),
                        tr("No Image Selected."),
                        QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                    return;

                }
                else {
                    setMandatoryField(ui->greyScaleImageSelector, false);

                    ui->voxel2meshBtn->setVisible(false);
                    ui->mesher_progressBar->setVisible(true);
                    ui->mesher_progressBar->setValue(25);

                    voxel2mesh_filter.SetUseGuassianSmoothing(ui->medianGroup->isChecked());
                    voxel2mesh_filter.SetMedianKernelSize(ui->kernelXSpinBox->value(),
                        ui->kernelYSpinBox->value(), ui->kernelZSpinBox->value());
                    voxel2mesh_filter.SetUseGuassianSmoothing(ui->gaussGroup->isChecked());
                    voxel2mesh_filter.SetGaussianStandardDeviation(ui->deviationSpinBox->value());
                    voxel2mesh_filter.SetGaussianRadius(ui->radiusSpinBox->value());
                    voxel2mesh_filter.SetPolygonSmoothing(ui->smoothingGroup->isChecked());
                    voxel2mesh_filter.SetIteration(ui->iterationSpinBox->value());
                    voxel2mesh_filter.SetRelaxationFactor(ui->relaxationSpinBox->value());
                    voxel2mesh_filter.SetIsovalue(ui->isovalueSpinBox->value());

                    voxel2mesh_filter.SetInputData(item.image_data);
                    break;
                }
            }
        }
    }

    ui->mesher_progressBar->setValue(50);

    voxel2mesh_filter.Update();

    vtkPolyData* mesh = voxel2mesh_filter.GetOutput();

    ui->mesher_progressBar->setValue(75);

    vtkSmartPointer<vtkPolyDataMapper> mesh_mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mesh_mapper->SetInputData(mesh);
    mesh_mapper->ScalarVisibilityOff();

    vtkSmartPointer<vtkActor> mesh_actor =
        vtkSmartPointer<vtkActor>::New();
    mesh_actor->SetMapper(mesh_mapper);
    //mesh_actor->GetProperty()->SetColor(250 / 250.0, 187 / 250.0, 124 / 250.0);
    //mesh_actor->GetProperty()->SetColor(250 / 250.0, 0.0, 0.0);
    mesh_actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    mesh_actor->GetProperty()->SetOpacity(1);

    renderer3D_->AddActor(mesh_actor);
    renderer3D_->ResetCamera();

    this->ui->view4->GetRenderWindow()->Render();

    ui->mesher_progressBar->setValue(100);
    ui->voxel2meshBtn->setVisible(true);
    ui->mesher_progressBar->setVisible(false);
}





//void MainWindow::on_pushButton_4_clicked()
//{
//    // TODO: start fusion
//    FusionParams fusion_params;
//    //fusion_params.img0 = ui->in_fusion_0_img0->currentText().toStdString();
//   // fusion_params.img1 = ui->in_fusion_1_img1->currentText().toStdString();
//    //fusion_params.opacity = double(ui->in_fusion_2_opacity->value())/100.0;
//    //fusion_params.fused_img_name = ui->in_fusion_3_fused_img_name->text().toStdString();
//}

//void MainWindow::on_in_fusion_2_opacity_valueChanged(int value)
//{
//    //ui->out_fusion_0_opacity->setText(QString::number(double(value)/100.0));
//}

//void MainWindow::on_pushButton_5_clicked()
//{
////    TODO: Load fixed image
//    QString fileName = QFileDialog::getOpenFileName(
//            this,
//            tr("open a file."),
//            "D:/",
//            tr("All files(*.*)"));
//    //ui->in_registration_0_fixed_img_name->setText(fileName);
//}

//void MainWindow::on_pushButton_6_clicked()
//{
////    TODO: Load moving image
//    QString fileName = QFileDialog::getOpenFileName(
//            this,
//            tr("open a file."),
//            "D:/",
//            tr("All files(*.*)"));
//    //ui->in_registration_1_moving_img_name->setText(fileName);
//}

//void MainWindow::on_pushButton_7_clicked()
//{
////    TODO: Registration
//    RegistrationParams registration_params;
//   /* registration_params.fixed_img_name = ui->in_registration_0_fixed_img_name->text().toStdString();
//    registration_params.moving_img_name = ui->in_registration_1_moving_img_name->text().toStdString();
//    registration_params.registration_type = RegistrationType(ui->in_registration_4_regi_type->currentIndex());
//    registration_params.metrics_type = MetricsType(ui->in_registration_5_metrics_type->currentIndex());*/
//}

//void MainWindow::on_pushButton_clicked()
//{
////    TODO: Generated surface
//    //ProcessParams process_params;
//    //process_params.img_name = ui->in_gray_0_img->currentText().toStdString();
//    //process_params.isosurface_value = ui->in_gray_1_isosurface_value->value();
//    //process_params.surface_name = ui->in_gray_2_surface_name->text().toStdString();

//}

// general processing
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

    if (ui->smooth_selector->count() == 0)
    {
        setMandatoryField(ui->smooth_selector, true);

        QMessageBox::warning(nullptr,
            tr("Error"),
            tr("No Image Selected."),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        return;
    }

    vtkSmartPointer<vtkImageData> image_smooth_result;

    // loop for finding corresponding ImageDataItem
    QString current_name = ui->smooth_selector->currentText();

    for (vector<ImageDataItem>& vec : image_tree_) {
        for (ImageDataItem& item : vec) {
            if (current_name == item.image_name) {
                if (item.image_data == nullptr) {
                    setMandatoryField(ui->smooth_selector, true);
                    QMessageBox::warning(nullptr,
                        tr("Error"),
                        tr("No Image Selected."),
                        QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                    return;
                }
                else {
                    // TODO : 
                    setMandatoryField(ui->smooth_selector, false);

                    image_smooth_result = image_smoothing(item.image_data.GetPointer(), params);

                    if (image_smooth_result == nullptr)
                    {
                        QMessageBox::warning(nullptr,
                            tr("Error"),
                            tr("Image is NULL."),
                            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                        return;
                    }

                    QString item_name = "smooth_" + current_name;
                    for (vector<ImageDataItem>& vec : image_tree_) {
                        for (ImageDataItem& item : vec) {
                            if (item_name == item.image_name) {
                                item_name = "(1)" + item_name;
                            }
                        }
                    }
                    vtk_image_collection_.push_back(image_smooth_result);

                    ImageDataItem image_item;
                    image_item.image_name = QString(item_name);
                    image_item.image_data = image_smooth_result;

                    // TODO: judge if the selected item is the parent item
                    vec.push_back(image_item);

                    break;
                }
            }
        }
    }

    update_data_manager();
}

void MainWindow::on_detect_edge_button_clicked()
{

    if (ui->edge_selector->count() == 0)
    {
        setMandatoryField(ui->edge_selector, true);

        QMessageBox::warning(nullptr,
            tr("Error"),
            tr("No Image Selected."),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        return;
    }


    vtkSmartPointer<vtkImageData> image_edge_result;

    // loop for finding corresponding ImageDataItem
    QString current_name = ui->edge_selector->currentText();

    for (vector<ImageDataItem>& vec : image_tree_) {
        for (ImageDataItem& item : vec) {
            if (current_name == item.image_name) {
                if (item.image_data == nullptr) {
                    setMandatoryField(ui->edge_selector, true);
                    QMessageBox::warning(nullptr,
                        tr("Error"),
                        tr("No Image Selected."),
                        QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                    return;
                }
                else {
                    // TODO : 
                    setMandatoryField(ui->edge_selector, false);

                    image_edge_result = image_detect_edge(item.image_data.GetPointer());

                    if (image_edge_result == nullptr)
                    {
                        QMessageBox::warning(nullptr,
                            tr("Error"),
                            tr("Image is NULL."),
                            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                        return;
                    }

                    QString item_name = "edge_" + current_name;
                    for (vector<ImageDataItem>& vec : image_tree_) {
                        for (ImageDataItem& item : vec) {
                            if (item_name == item.image_name) {
                                item_name = "(1)" + item_name;
                            }
                        }
                    }
                    vtk_image_collection_.push_back(image_edge_result);

                    ImageDataItem image_item;
                    image_item.image_name = QString(item_name);
                    image_item.image_data = image_edge_result;

                    // TODO: judge if the selected item is the parent item
                    vec.push_back(image_item);

                    break;
                }
            }
        }
    }

    update_data_manager();


}

void MainWindow::on_start_thresholding_button_clicked()
{
    ThresholdingParams params;
    params.lower_value = this->ui->in_lower_value->value();
    params.upper_value = this->ui->in_upper_value->value();

    if (ui->thre_selector->count() == 0)
    {
        setMandatoryField(ui->thre_selector, true);

        QMessageBox::warning(nullptr,
            tr("Error"),
            tr("No Image Selected."),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        return;
    }


    vtkSmartPointer<vtkImageData> image_threshold_result;

    // loop for finding corresponding ImageDataItem
    QString current_name = ui->thre_selector->currentText();

    for (vector<ImageDataItem>& vec : image_tree_){
        for (ImageDataItem& item : vec){
            if (current_name == item.image_name){
                if (item.image_data == nullptr){
                    setMandatoryField(ui->thre_selector, true);
                    QMessageBox::warning(nullptr,
                        tr("Error"),
                        tr("No Image Selected."),
                        QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                    return;
                }
                else {
                    // TODO : 
                    setMandatoryField(ui->thre_selector, false);

                    image_threshold_result = image_threshold(item.image_data.GetPointer(), params);
                    
                    if (image_threshold_result == nullptr)
                    {
                        QMessageBox::warning(nullptr,
                            tr("Error"),
                            tr("Image is NULL."),
                            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                        return;
                    }

                    QString item_name = "thresholding_" + current_name;
                    for (vector<ImageDataItem>& vec : image_tree_){
                        for (ImageDataItem& item : vec){
                            if (item_name == item.image_name){
                                item_name = "(1)" + item_name;
                            }
                        }
                    }
                    vtk_image_collection_.push_back(image_threshold_result);

                    ImageDataItem image_item;
                    image_item.image_name = QString(item_name);
                    image_item.image_data = image_threshold_result;

                    // TODO: judge if the selected item is the parent item
                    vec.push_back(image_item);

                    break;
                }
            }
        }
    }

    update_data_manager();
}

vtkSmartPointer<vtkImageData> MainWindow::image_detect_edge(vtkImageData* input_image)
{
    vtkSmartPointer<vtkImageGradientMagnitude> gradient_magnitude_filter =
        vtkSmartPointer<vtkImageGradientMagnitude>::New();
    gradient_magnitude_filter->SetInputData(input_image);
    gradient_magnitude_filter->Update();

    vtkSmartPointer<vtkImageCast> gradient_magnitude_cast_filter =
        vtkSmartPointer<vtkImageCast>::New();
    gradient_magnitude_cast_filter->SetInputConnection(gradient_magnitude_filter->GetOutputPort());
    gradient_magnitude_cast_filter->SetOutputScalarTypeToUnsignedChar();
    gradient_magnitude_cast_filter->Update();

    return gradient_magnitude_cast_filter->GetOutput();
}

vtkSmartPointer<vtkImageData> MainWindow::image_threshold(vtkImageData* input_image, ThresholdingParams params)
{
    vtkSmartPointer<vtkImageThreshold> threshold_filter = vtkSmartPointer<vtkImageThreshold>::New();
    threshold_filter->SetInputData(input_image);
    threshold_filter->ThresholdBetween(params.lower_value, params.upper_value);
    threshold_filter->ReplaceInOn();
    threshold_filter->ReplaceOutOn();
    threshold_filter->SetInValue(255);
    threshold_filter->SetOutValue(0);
    threshold_filter->Update();

    return threshold_filter->GetOutput();
}

vtkSmartPointer<vtkImageData> MainWindow::image_smoothing(vtkImageData* input_image, SmoothingParams params)
{
    switch (params.smooth_type) 
    {
    case 0:
    {
        vtkSmartPointer<vtkImageGaussianSmooth> gaussian_smooth_filter =
            vtkSmartPointer<vtkImageGaussianSmooth>::New();
        gaussian_smooth_filter->SetInputData(input_image);
        gaussian_smooth_filter->SetDimensionality(3);
        gaussian_smooth_filter->SetRadiusFactor(params.kernel_size);
        gaussian_smooth_filter->SetStandardDeviation(params.sigma_x);
        gaussian_smooth_filter->Update();

        return gaussian_smooth_filter->GetOutput();
    }
    case 1:
    {
        vtkSmartPointer<vtkImageConvolve> convolve_filter =
            vtkSmartPointer<vtkImageConvolve>::New();
        convolve_filter->SetInputData(input_image);

        switch (params.kernel_size) 
        {
        case 3: {
            double kernel[27];
            for (int i = 0; i <27; i++)
            {
                kernel[i] = 1.0 / 27.0;
            }
            convolve_filter->SetKernel3x3x3(kernel);
            break;
        }
        case 5: {
            double kernel[125];
            for (int i = 0; i < 125; i++)
            {
                kernel[i] = 1.0 / 125.0;
            }
            convolve_filter->SetKernel5x5x5(kernel);
            break;
        }
        case 7: {
            double kernel[343];
            for (int i = 0; i < 343; i++)
            {
                kernel[i] = 1.0 / 343.0;
            }
            convolve_filter->SetKernel7x7x7(kernel);
            break;
        }
        }

        convolve_filter->Update();
        return convolve_filter->GetOutput();
    }
    case 2:
    {
        vtkSmartPointer<vtkImageMedian3D> median_filter =
            vtkSmartPointer<vtkImageMedian3D>::New();
        median_filter->SetInputData(input_image);
        median_filter->SetKernelSize(params.kernel_size, params.kernel_size, params.kernel_size);
        median_filter->Update();

        return median_filter->GetOutput();
    }
    default:
        return nullptr;
    }
}


// Data Manager
/*
 * update_data_manager: update ui->data_manager according to this->image_tree_
 * image_tree_ is two layer: 1. folder 2. images, eg: image_tree_[0][0] saves the original image of first folder we opened
 * cur_selected_image_ind saves the index, !!! make sure to check if cur_selected_image_ind is >= 0 before using it !!!
 * cur_selected_image_ind[0] = -1 means there is no selected image in current stage
 */
void MainWindow::update_data_manager() {
    if (image_tree_.empty()) {
        return;
    }

    // Clears the tree widget by removing all of its items and selections.
    ui->data_manager->clear();
    QTreeWidgetItem *folder_name;
    for (vector<ImageDataItem> &vec : image_tree_) 
    {
        if (vec.empty()) 
        {
            continue;
        }

        // vec[0]: First ImageDataItem Node, set as the parent node for other items
        folder_name = new QTreeWidgetItem(ui->data_manager, QStringList(vec[0].image_name));
        folder_name->setCheckState(0, Qt::Unchecked);

        int len = int(vec.size());  // the number of ImageDataItems
        for (int i = 1; i < len; i++) 
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(folder_name, QStringList(vec[i].image_name));
            item->setCheckState(0, Qt::Unchecked);
        }

    }

    // set the new node checked
    folder_name->setCheckState(0, Qt::Checked);

    cur_selected_image_ind_[0] = int(image_tree_.size())-1;
    cur_selected_image_ind_[1] = 0;

    this->ui->data_manager->expandAll();

//    update combo box
    ui->FixedImageSelector->clear();
    ui->MovingImageSelector->clear();
//    ui->maskImageSelector->clear();
    ui->greyScaleImageSelector->clear();
    ui->BlendImage0Selector->clear();
    ui->BlendImage1Selector->clear();
    ui->smooth_selector->clear();
    ui->edge_selector->clear();
    ui->thre_selector->clear();
    ui->seg_image_selector->clear();

    for (vector<ImageDataItem> &vec: image_tree_)
    {
        for (ImageDataItem &item: vec) 
        {
            ui->FixedImageSelector->addItem(item.image_name);
            ui->MovingImageSelector->addItem(item.image_name);
//            ui->maskImageSelector->addItem(item.image_name);
            ui->greyScaleImageSelector->addItem(item.image_name);
            ui->BlendImage0Selector->addItem(item.image_name);
            ui->BlendImage1Selector->addItem(item.image_name);
            ui->smooth_selector->addItem(item.image_name);
            ui->edge_selector->addItem(item.image_name);
            ui->thre_selector->addItem(item.image_name);
            ui->seg_image_selector->addItem(item.image_name);
        }
    }
}

/*
 * on_data_manager_itemClicked: triggered when item is clicked
 * [in]   item: the item user clicked
 * [in] column:
 * make sure single checked item in data_manager
 */
void MainWindow::on_data_manager_itemClicked(QTreeWidgetItem *item, int column)
{
    if (item->checkState(column) == Qt::Checked) 
    {
        QTreeWidgetItemIterator iter(ui->data_manager);
        while (*iter) {
//            qDebug()<<(*iter)->text(0);
            (*iter)->setCheckState(0, Qt::Unchecked);
            ++iter;
        }
        item->setCheckState(column, Qt::Checked);
        if (item->parent() == nullptr) {
            cur_selected_image_ind_[0] = ui->data_manager->indexOfTopLevelItem(item);
            cur_selected_image_ind_[1] = 0;
        } else {
            cur_selected_image_ind_[0] = ui->data_manager->indexOfTopLevelItem(item->parent());
            cur_selected_image_ind_[1] = item->parent()->indexOfChild(item)+1;
        }

    } else {
        cur_selected_image_ind_[0] = -1;
        

    }
    qDebug()<<"data manager item clicked index: ("<<cur_selected_image_ind_[0]<<","
           <<cur_selected_image_ind_[1]<<"), name["<<item->text(column)<<"]";

    show_image();
    volume_rendering(false);
    ui->action_visualization->setChecked(false);
}


void MainWindow::clear_manager()
{
    image_tree_.clear();
    ui->data_manager->clear();
    //vtk_image_collection_.clear();
    //itk_image_collection_.clear();

    ui->FixedImageSelector->clear();
    ui->MovingImageSelector->clear();
    //    ui->maskImageSelector->clear();
    ui->greyScaleImageSelector->clear();
    ui->BlendImage0Selector->clear();
    ui->BlendImage1Selector->clear();
    ui->smooth_selector->clear();
    ui->edge_selector->clear();
    ui->thre_selector->clear();
    ui->seg_image_selector->clear();

    this->ui->view1->hide();
    this->ui->view2->hide();
    this->ui->view3->hide();

    cur_selected_image_ind_[0] = -1;

    volume_rendering(false);
    ui->action_visualization->setChecked(false);
}

void MainWindow::on_addPatientBtn_clicked()
{
//    TODO: add patient
    PatientFormParams params;
    params.user_info = user;
    PatientForm patient_form(params, this);
    int ret = patient_form.exec();
    if (ret == QDialog::Accepted) {
        update_patients();
    }
}

void MainWindow::on_action_upload_file_triggered()
{
    UploadFormParams params;
//    params.communicator = communicator;
    params.user_info = user;
//    params.patients = patients_;
    UploadForm upload_form(params, this);
    upload_form.exec();
}

void MainWindow::on_action_download_file_triggered()
{
    DownloadFormParams params;
    params.user_info = user;
//    params.patients = patients_;
    DownloadForm download_form(params, this);
    download_form.exec();
}

void MainWindow::init()
{
//    init patient
    update_patients();
}

void MainWindow::on_patientSelector_currentTextChanged(const QString &arg1)
{
    qDebug()<<"patient selected: "<<arg1;
    image_requester.setToken(user._token());
    QVector<QString> image_names = image_requester.getCtimeHttp(arg1);
    ui->patientImageSelector->clear();
    for (QString &name: image_names) {
        ui->patientImageSelector->addItem(name);
    }
    int cur_patient_index = ui->patientSelector->currentIndex();
    if (cur_patient_index < 0 || cur_patient_index >= patients_.size()) {
        return;
    }
    auto pat = patients_[cur_patient_index];
    ui->patientGenderLabel->setText(pat._age()?"Female":"Male");
    ui->patientBirthLabel->setText(pat._birth());
    ui->patientAgeLabel->setText(QString::number(pat._age()));
}

void MainWindow::update_patients()
{
    patient temp_patient;
    temp_patient.set_token(user._token());
    this->patients_ = temp_patient.http_get_all_patient(&communicator);
    ui->patientSelector->clear();
    for (patient &pat: patients_) {
//        qDebug()<<pat._id()<<":"<<pat._name();
        ui->patientSelector->addItem(pat._name());
    }
    if (patients_.empty()) {
        return;
    }
    patient pat = patients_[0];
    ui->patientGenderLabel->setText(pat._gender() ? "Female" : "Male");
    ui->patientBirthLabel->setText(pat._birth());
    ui->patientAgeLabel->setText(QString::number(pat._age()));
}



void MainWindow::setMandatoryField(QWidget* widget, bool bEnabled) {
    setQStyleSheetField(widget, "mandatoryField", bEnabled);
}

void MainWindow::setQStyleSheetField(QWidget* widget, const char* fieldName, bool bEnabled) {
    widget->setProperty(fieldName, bEnabled);
    widget->style()->unpolish(widget); // need to do this since we changed the stylesheet
    widget->style()->polish(widget);
    widget->update();
}
