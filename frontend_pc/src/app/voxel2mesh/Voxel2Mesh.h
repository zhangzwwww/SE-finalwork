#ifndef _VOXEL2MESH_h__
#define _VOXEL2MESH_h__

#include <vtkSmartPointer.h>

class vtkImageData;
class vtkPolyData;

class Voxel2Mesh
{
public:
    Voxel2Mesh();
    ~Voxel2Mesh();

    void SetInputData(vtkImageData* inputImage);
    void Update();
    vtkPolyData* GetOutput();

    // Setters
    void SetUseMedian(bool bMedian);
    void SetUseGuassianSmoothing(bool bGaussian);
    void SetPolygonSmoothing(bool bSmooth);
    void SetMedianKernelSize(int x, int y, int z);
    void SetGaussianStandardDeviation(double standardDeviation);
    void SetGaussianRadius(double radius);
    void SetIsovalue(double isovalue);
    void SetIteration(int iter);
    void SetRelaxationFactor(double factor);

private:
    vtkSmartPointer<vtkImageData> m_image;
    vtkSmartPointer<vtkPolyData> m_mesh;

    bool m_UseMedian, m_UseGaussianSmoothing, m_UsePolygonSmoothing;
    int m_MedianKernelSizeX, m_MedianKernelSizeY, m_MedianKernelSizeZ;
    double m_GaussianStandardDeviation, m_GaussianRadius, m_Isovalue;
    int m_Iterations;
    double m_RelaxationFactor;

};

#endif //_VOXEL2MESH_h__
