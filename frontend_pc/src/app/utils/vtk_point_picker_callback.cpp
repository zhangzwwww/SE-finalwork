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

        //vector_picked[cur_view].push_back({picked[0], picked[1], picked[2]});
        if (vector_displaypos[cur_view].empty()) {
            vector_displaypos[cur_view].push_back({ start_pos[0], start_pos[1] });
        }
        else {
            int last_x = int(vector_displaypos[cur_view][0][0]);
            int last_y = int(vector_displaypos[cur_view][0][1]);
            if (last_x != int(start_pos[0]) || last_y != start_pos[1]) {
                vector_displaypos[cur_view].push_back({ start_pos[0], start_pos[1] });
            }
        }

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

        //qDebug() << "release:" <<  vector_picked[cur_view].size();
        qDebug() << "release:" << vector_displaypos[cur_view];

        auto render = this->view[cur_view]->GetRenderer();

        vtkActorCollection* actorCollection = render->GetActors();
        int num = actorCollection->GetNumberOfItems();
        actorCollection->InitTraversal();
        //vtkActor* actortemp = actorCollection->GetNextActor();
        if (vector_displaypos[cur_view].size() == 2)
        {
            //QMessageBox message_box(QMessageBox::NoIcon, "Upload Mark", "Upload this mark?", QMessageBox::Yes | QMessageBox::No);
            //int choice = message_box.exec();
            //if (choice == QMessageBox::Yes) {
            //    UploadMark(folder_path_);
            //} else {
            //    RefreshMark();
            //}
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

