#include "vtk_add_seed_callback.h"

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


vtkAddSeedCallback::vtkAddSeedCallback() {
    is_adding = false;
    cur_view = -1;
}

void vtkAddSeedCallback::Execute(vtkObject* caller, unsigned long ev,
    void* callData)
{
    if (is_adding == false) {
        return;
    }
    if (!(ev == vtkCommand::RightButtonPressEvent)) 
    {
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

        start_pos[0] = display_pos[0];
        start_pos[1] = display_pos[1];

        auto render = this->view[cur_view]->GetRenderer();
        interactor[cur_view]->GetPicker()->Pick(start_pos[0],
            start_pos[1],
            0,  // always zero.
            render);

        double picked[3];
        double pos_display[3] = { double(start_pos[0]), double(start_pos[1]), 0.0 };

        render->SetDisplayPoint(pos_display);
        render->DisplayToWorld();
        render->GetWorldPoint(picked);

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
        this->view[cur_view]->Render();
        
        SegmentationWorker::IndexType seed;
        seed[0] = (picked[0] - origin[0]) / spacing[0];
        seed[1] = (picked[1] - origin[1]) / spacing[1];
        seed[2] = (picked[2] - origin[2]) / spacing[2];

        vector_seeds.push_back(seed);

    }
  
    return;
}

void vtkAddSeedCallback::StartAdd() {
    is_adding = true;
}

void vtkAddSeedCallback::EndAdd() {
    RefreshSeed();
    is_adding = false;
}

void vtkAddSeedCallback::RefreshSeed() {
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

void vtkAddSeedCallback::GetScreentPos(double displayPos[2], double world[2])
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

void vtkAddSeedCallback::DrawPoint(double pos[3]) {
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

