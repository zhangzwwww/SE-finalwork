#ifndef VTK_POINT_PICKER_CALLBACK_H
#define VTK_POINT_PICKER_CALLBACK_H

#include <vector>

#include <QString>

#include <vtkObject.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include "commu/imageinfo.h"
#include "commu/userinfo.h"

using std::vector;

class vtkPointPickerCallback : public vtkCommand
{
public:
    static vtkPointPickerCallback* New()
    {
        return new vtkPointPickerCallback;
    }
    vtkPointPickerCallback();
    void Execute(vtkObject* caller, unsigned long ev,
        void* callData);
    vtkSmartPointer<vtkActor> SetLine(double start_point[], double end_point[]);
    void DrawRect();
    void StartMark();
    void EndMark();
    void RefreshMark();
    void UploadMark(QString folder_path);
    void GetScreentPos(double displayPos[2], double world[2]);
    void DrawPoint(double pos[3]);
    void SetFolderPath(QString folder_path);

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
    imageInfo *image_manager_;
    QString folder_path_;
};

#endif // VTK_POINT_PICKER_CALLBACK_H
