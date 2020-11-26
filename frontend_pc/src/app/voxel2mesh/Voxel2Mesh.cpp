#include "Voxel2Mesh.h"

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkMarchingCubes.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkImageMedian3D.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyDataNormals.h>


Voxel2Mesh::Voxel2Mesh()
    : m_UseMedian(false),
    m_MedianKernelSizeX(3),
    m_MedianKernelSizeY(3),
    m_MedianKernelSizeZ(3),
    m_UseGaussianSmoothing(true),
    m_GaussianStandardDeviation(2.2),
    m_GaussianRadius(0.49),
	m_UsePolygonSmoothing(true),
	m_Iterations(50),
	m_RelaxationFactor(0.1){
};

Voxel2Mesh::~Voxel2Mesh()
{
	//m_image->Delete();
	//m_mesh->Delete();
}


void Voxel2Mesh::SetInputData(vtkImageData* inputImage)
{
    m_image = inputImage;
}

vtkPolyData* Voxel2Mesh::GetOutput()
{
    return m_mesh;
}


void Voxel2Mesh::Update()
{
	if (m_image == nullptr)
	{
		return;
	}

    if (m_UseMedian)
    {
        vtkSmartPointer<vtkImageMedian3D> median =
            vtkSmartPointer<vtkImageMedian3D>::New();
        median->SetInputData(m_image);
        median->SetKernelSize(m_MedianKernelSizeX, m_MedianKernelSizeY, m_MedianKernelSizeZ);
        median->ReleaseDataFlagOn();
        median->UpdateInformation();
        median->Update();
        m_image = median->GetOutput();
    }

    if (m_UseGaussianSmoothing) 
    {
        vtkSmartPointer<vtkImageGaussianSmooth> gaussian =
            vtkSmartPointer<vtkImageGaussianSmooth>::New();
        gaussian->SetInputData(m_image);
        gaussian->SetDimensionality(3);
        gaussian->SetRadiusFactor(m_GaussianRadius);
        gaussian->SetStandardDeviation(m_GaussianStandardDeviation);
        gaussian->ReleaseDataFlagOn();
        gaussian->UpdateInformation();
        gaussian->Update();
        m_image = gaussian->GetOutput();
    }

	// MarchingCube -->create Surface
	vtkSmartPointer<vtkMarchingCubes> marchingCubesFilter =
		vtkSmartPointer<vtkMarchingCubes>::New();
	marchingCubesFilter->ComputeScalarsOff();
	marchingCubesFilter->SetInputData(m_image);
	marchingCubesFilter->SetValue(0, m_Isovalue);
	marchingCubesFilter->Update();

	vtkSmartPointer<vtkPolyData> polydata =
		vtkSmartPointer<vtkPolyData>::New();
	polydata = marchingCubesFilter->GetOutput();

	vtkSmartPointer<vtkPolyDataConnectivityFilter> confilter =
		vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	confilter->SetInputData(polydata);
	confilter->SetExtractionModeToLargestRegion();
	confilter->Update();

	polydata = confilter->GetOutput();

	if (m_UsePolygonSmoothing)
	{
		vtkSmartPointer<vtkSmoothPolyDataFilter> smoother =
			vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
		smoother->SetInputData(polydata);
		smoother->SetNumberOfIterations(m_Iterations);
		smoother->SetRelaxationFactor(m_RelaxationFactor);
		smoother->SetFeatureAngle(60);
		smoother->FeatureEdgeSmoothingOff();
		smoother->BoundarySmoothingOff();
		smoother->SetConvergence(0);
		smoother->Update();

		polydata = smoother->GetOutput();
	}


	vtkSmartPointer<vtkPolyDataNormals> normalsGenerator =
		vtkSmartPointer<vtkPolyDataNormals>::New();
	normalsGenerator->SetInputData(polydata);
	normalsGenerator->SetComputeCellNormals(1);
	normalsGenerator->SetComputePointNormals(1);
	normalsGenerator->FlipNormalsOn();

	vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter =
		vtkSmartPointer<vtkCleanPolyData>::New();
	cleanPolyDataFilter->SetInputConnection(
		normalsGenerator->GetOutputPort());
	cleanPolyDataFilter->PieceInvariantOff();
	cleanPolyDataFilter->ConvertLinesToPointsOff();
	cleanPolyDataFilter->ConvertPolysToLinesOff();
	cleanPolyDataFilter->ConvertStripsToPolysOff();
	cleanPolyDataFilter->PointMergingOn();
	cleanPolyDataFilter->Update();

	m_mesh = cleanPolyDataFilter->GetOutput();

};


void Voxel2Mesh::SetUseMedian(bool bMedian)
{
    m_UseMedian = bMedian;
};

void Voxel2Mesh::SetUseGuassianSmoothing(bool bGaussian) 
{
    m_UseGaussianSmoothing = bGaussian;
};

void Voxel2Mesh::SetPolygonSmoothing(bool bSmooth)
{
	m_UsePolygonSmoothing = bSmooth;
};


void Voxel2Mesh::SetMedianKernelSize(int x, int y, int z) 
{
    m_MedianKernelSizeX = x;
    m_MedianKernelSizeY = y;
    m_MedianKernelSizeZ = z;
}

void Voxel2Mesh::SetGaussianStandardDeviation(double standardDeviation)
{
    m_GaussianStandardDeviation = standardDeviation;
}

void Voxel2Mesh::SetGaussianRadius(double radius) 
{
    m_GaussianRadius = radius;
}

void Voxel2Mesh::SetIsovalue(double isovalue) 
{
    m_Isovalue = isovalue;
}


void Voxel2Mesh::SetIteration(int iter)
{
	m_Iterations = iter;
}

void Voxel2Mesh::SetRelaxationFactor(double factor)
{
	m_RelaxationFactor = factor;
}

