#ifndef VTK_ADD_SEED_CALLBACK_H
#define VTK_ADD_SEED_CALLBACK_H

#include <vector>
#include <QString>
#include <vtkObject.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <segmentation_worker.h>

using std::vector;

class vtkAddSeedCallback : public vtkCommand
{
public:
    static vtkAddSeedCallback* New()
    {
        return new vtkAddSeedCallback;
    }
    vtkAddSeedCallback();
    void Execute(vtkObject* caller, unsigned long ev,
        void* callData);
   
    void StartAdd();
    void EndAdd();
    void RefreshSeed();
    void DrawPoint(double pos[3]);
    void GetScreentPos(double displayPos[2], double world[2]);

    vtkImageViewer2* view[3];
    vtkRenderWindowInteractor* interactor[3];
    vtkRenderer* render[3];


    std::vector<SegmentationWorker::IndexType> vector_seeds;
    vector<vector<int>> vector_displaypos[3];
    int cur_view;
    int cur_slice;
    int start_pos[2];
    int end_pos[2];
    bool is_adding;
    double spacing[3];
    double origin[3];

};

#endif // VTK_ADD_SEED_CALLBACK_H
