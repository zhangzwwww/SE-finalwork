#include "vtk_point_picker_callback.h"

#include <algorithm>

#include <QDebug>
#include <QMessageBox>

#include <vtkInteractorStyleImage.h>
#include <vtkInteractorStyleRubberBand2D.h>
#include <vtkLineSource.h>
#include <vtkCoordinate.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkAbstractPicker.h>
#include <vtkProperty.h>

#include "../app/errorcode.h"

vtkPointPickerCallback::vtkPointPickerCallback() {
    is_drawing = false;
    is_marking = false;
    cur_view = -1;
    for (int i = 0; i < 4; i++) {
        actors[i] = vtkSmartPointer<vtkActor>::New();
    }
}

void vtkPointPickerCallback::Execute(vtkObject* caller, unsigned long ev,
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
        vector_displaypos[cur_view].push_back({ start_pos[0], start_pos[1] });

        vtkSmartPointer<vtkSphereSource> sphereSource =
            vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->Update();

        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(sphereSource->GetOutputPort());
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->SetPosition(picked);
        actor->SetScale(5);
        actor->SetDragable(true);
        actor->SetPickable(true);
        actor->GetProperty()->SetColor(0.0, 1.0, 0.0);

        render->AddActor(actor);
        //render->Render();

        this->view[cur_view]->Render();
    }
    else if (ev == vtkCommand::RightButtonReleaseEvent) {
        is_drawing = false;
        //for (int i = 0; i < 4; i++) {
        //    this->view[cur_view]->GetRenderer()->RemoveActor(actors[i]);
        //}

        // qDebug() << "release:" <<  vector_picked[cur_view].size();

        auto render = this->view[cur_view]->GetRenderer();

        vtkActorCollection* actorCollection = render->GetActors();
        int num = actorCollection->GetNumberOfItems();
        actorCollection->InitTraversal();
        //vtkActor* actortemp = actorCollection->GetNextActor();
        if (vector_displaypos[cur_view].size() == 2)
        {
            //auto start_vec = vector_displaypos[cur_view].at(0);
            //double start[2];
            //start[0] = start_vec.at(0); start[1] = start_vec.at(1);

            //auto end_vec = vector_displaypos[cur_view].at(1);
            //double end[2];
            //end[0] = end_vec.at(0); end[1] = end_vec.at(1);


            //double point1[2];
            //double point2[2];
            //double point3[2];
            //double point4[2];

            //double left[2];
            //double right[2];

            //left[0] = start[0] <= end[0] ? start[0] : end[0];
            //left[1] = start[1] <= end[1] ? start[1] : end[1];

            //right[0] = start[0] > end[0] ? start[0] : end[0];
            //right[1] = start[1] > end[1] ? start[1] : end[1];

            //point1[0] = left[0];  point1[1] = left[1];  //point1[2] = 0.0;
            //point2[0] = left[0];  point2[1] = right[1]; //point2[2] = 0.0;
            //point3[0] = right[0]; point3[1] = right[1]; //point3[2] = 0.0;
            //point4[0] = right[0]; point4[1] = left[1];  //point4[2] = 0.0;

            ////double p1_world[3];
            ////double p2_world[3];
            ////double p3_world[3];
            ////double p4_world[3];

            //qDebug() << "Right: " << right[0] << right[1];
            //qDebug() << "Left:  " << left[0] << left[1];

            //qDebug() << "point1: " << point1[0] << point1[1];
            //qDebug() << "point2:  " << point2[0] << point2[1];
            //qDebug() << "point3: " << point3[0] << point3[1];
            //qDebug() << "point4:  " << point4[0] << point4[1];

            ////for (int i = 0; i < 4; i++)

            //vtkSmartPointer<vtkCoordinate> pCoorPress1 = vtkSmartPointer<vtkCoordinate>::New();
            //pCoorPress1->SetCoordinateSystemToDisplay();
            //pCoorPress1->SetValue(point1);
            //double* p1_world = pCoorPress1->GetComputedWorldValue(render);

            //vtkSmartPointer<vtkCoordinate> pCoorPress2 = vtkSmartPointer<vtkCoordinate>::New();
            //pCoorPress2->SetCoordinateSystemToDisplay();
            //pCoorPress2->SetValue(point2);
            //double* p2_world = pCoorPress2->GetComputedWorldValue(render);

            //vtkSmartPointer<vtkCoordinate> pCoorPress3 = vtkSmartPointer<vtkCoordinate>::New();
            //pCoorPress3->SetCoordinateSystemToDisplay();
            //pCoorPress3 ->SetValue(point3);
            //double* p3_world = pCoorPress3->GetComputedWorldValue(render);


            //vtkSmartPointer<vtkCoordinate> pCoorPress4 = vtkSmartPointer<vtkCoordinate>::New();
            //pCoorPress4->SetCoordinateSystemToDisplay();
            //pCoorPress4->SetValue(point4);
            //double* p4_world = pCoorPress4->GetComputedWorldValue(render);

            ////GetScreentPos(point1, p1_world, cur_view);

            ////GetScreentPos(point2, p2_world, cur_view);

            ////render->SetDisplayPoint(point1);
            ////render->DisplayToWorld();
            ////render->GetWorldPoint(p1_world);


            ////render->SetDisplayPoint(point2);
            ////render->DisplayToWorld();
            ////render->GetWorldPoint(p2_world);

            ////render->SetDisplayPoint(point3);
            ////render->DisplayToWorld();
            ////render->GetWorldPoint(p3_world);

            ////render->SetDisplayPoint(point4);
            ////render->DisplayToWorld();
            ////render->GetWorldPoint(p4_world);


            //qDebug() << p1_world[0] << p1_world[1] << p1_world[2];
            //qDebug() << p2_world[0] << p2_world[1] << p2_world[2];
            //qDebug() << p3_world[0] << p3_world[1] << p3_world[2];
            //qDebug() << p4_world[0] << p4_world[1] << p4_world[2];



            //auto actor1 = this->SetLine(p1_world, p2_world);
            //auto actor2 = this->SetLine(p2_world, p3_world);
            //auto actor3 = this->SetLine(p3_world, p4_world);
            //auto actor4 = this->SetLine(p4_world, p1_world);

            //if (actor1 == nullptr)
            //{
            //    return;
            //}

            ///*vtkSmartPointer<vtkLineSource> line_source = vtkSmartPointer<vtkLineSource>::New();
            //line_source->SetPoint1(point1);
            //line_source->SetPoint2(point2);
            //line_source->Update();

            //vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            //mapper->SetInputConnection(line_source->GetOutputPort());

            //vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
            //actor->SetMapper(mapper);
            //actor->GetProperty()->SetLineWidth(2);
            //actor->GetProperty()->SetColor(0.0, 1.0, 0.0);*/


            //render->AddActor(actor1);
            //render->AddActor(actor2);
            //render->AddActor(actor3);
            //render->AddActor(actor4);

            //this->view[cur_view]->Render();
//            upload mark
            QMessageBox message_box(QMessageBox::NoIcon, "Upload Mark", "Upload this mark?", QMessageBox::Yes | QMessageBox::No);
            int choice = message_box.exec();
            if (choice == QMessageBox::Yes) {
                UploadMark(folder_path_);
            } else {
                RefreshMark();
            }
        }
    }
    return;
}

vtkSmartPointer<vtkActor> vtkPointPickerCallback::SetLine(double start_point[], double end_point[])
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

void vtkPointPickerCallback::DrawRect() {
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

void vtkPointPickerCallback::StartMark() {
    is_marking = true;
}

void vtkPointPickerCallback::EndMark() {
    RefreshMark();
    is_marking = false;
}

void vtkPointPickerCallback::RefreshMark() {
    for (int i = 0; i < 3; i++) {
        vector_displaypos[i].clear();
        auto render = this->view[i]->GetRenderer();
        vtkActorCollection* actorCollection = render->GetActors();
        int num = actorCollection->GetNumberOfItems();
        actorCollection->InitTraversal();
        //vtkActor* actortemp = actorCollection->GetNextActor();
        for (int j = 0; j < num; ++j)
        {
            vtkActor* actortemp = actorCollection->GetNextActor();
            render->RemoveActor(actortemp);
        }
        this->view[i]->Render();
        render->ResetCamera();
    }
}

void vtkPointPickerCallback::UploadMark(QString folder_path) {
    if (vector_displaypos[cur_view].size() < 2 || vector_displaypos[cur_view][0].size() < 2 || vector_displaypos[cur_view][1].size() < 2) {
        return;
    }
    double left_top_x = double(std::min(vector_displaypos[cur_view][0][0], vector_displaypos[cur_view][1][0]));
    double left_top_y = double(std::min(vector_displaypos[cur_view][0][1], vector_displaypos[cur_view][1][1]));
    double right_bottom_x = double(std::max(vector_displaypos[cur_view][0][0], vector_displaypos[cur_view][1][0]));
    double right_bottom_y = double(std::max(vector_displaypos[cur_view][0][1], vector_displaypos[cur_view][1][1]));
    int ret = image_manager_->uploadImgMark(folder_path, cur_slice, cur_view, left_top_x, left_top_y, right_bottom_x, right_bottom_y);
    if (ret == SUCCESS) {
        QMessageBox::information(nullptr, "success", "Succeed to upload mark!", QMessageBox::Yes);
        RefreshMark();
        EndMark();
    } else {
        QMessageBox::warning(nullptr, "error", "Fail to upload mark!", QMessageBox::Yes);
    }
}

void vtkPointPickerCallback::SetFolderPath(QString folder_path) {
    folder_path_ = folder_path;
}

void vtkPointPickerCallback::GetScreentPos(double displayPos[2], double world[2])
{
//      vtkSmartPointer<vtkRenderer> renderer = this->view[cur_view]->GetRenderer();
//        renderer->SetDisplayPoint(displayPos);
//        renderer->DisplayToWorld();
//        renderer->GetWorldPoint(world);
    vtkSmartPointer<vtkCoordinate> pCoorPress = vtkSmartPointer<vtkCoordinate>::New();
    pCoorPress->SetCoordinateSystemToDisplay();
    pCoorPress->SetValue(displayPos);
    world = pCoorPress->GetComputedWorldValue(this->view[cur_view]->GetRenderer());
}

void vtkPointPickerCallback::DrawPoint(double pos[3]) {
    vtkSmartPointer<vtkSphereSource> sphere_source =
        vtkSmartPointer<vtkSphereSource>::New();
    sphere_source->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphere_source->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->SetPosition(pos);
    actor->SetScale(3);
    actor->SetDragable(true);
    actor->SetPickable(true);
    actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    this->view[cur_view]->GetRenderer()->AddActor(actor);
}

