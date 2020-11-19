#ifndef __RegistrationWorker_h__
#define __RegistrationWorker_h__

// ITK
#include <itkImage.h>
#include <itkCommand.h>
#include <itkVersorRigid3DTransform.h>
#include "itkImageRegistrationMethod.h"
#include "itkRigid3DTransform.h"
#include "itkMutualInformationImageToImageMetric.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkVersorRigid3DTransformOptimizer.h"
#include "itkCenteredTransformInitializer.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkNormalizeImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkGradientDescentOptimizer.h"
#include "itkTranslationTransform.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkCheckerBoardImageFilter.h"

#include <QObject>
#include <QRunnable>

class CommandIterationUpdate : public itk::Command
{
public:
    using Self = CommandIterationUpdate;
    using Superclass = itk::Command;
    using Pointer = itk::SmartPointer<Self>;
    itkNewMacro(Self);

protected:
    CommandIterationUpdate() = default;

public:
    using OptimizerType = itk::VersorRigid3DTransformOptimizer;
    using OptimizerPointer = const OptimizerType*;

    void
        Execute(itk::Object* caller, const itk::EventObject& event) override
    {
    
        Execute((const itk::Object*)caller, event);
    }

    void
        Execute(const itk::Object* object, const itk::EventObject& event) override
    {
        auto optimizer = dynamic_cast<OptimizerPointer>(object);
        if (!itk::IterationEvent().CheckEvent(&event))
        {
            return;
        }

        std::cout.precision(4);
        std::cout << optimizer->GetCurrentIteration() << "   ";
        std::cout << optimizer->GetValue() << "   ";
        std::cout << optimizer->GetCurrentStepLength() << "   ";
        std::cout << optimizer->GetCurrentPosition() << std::endl;

        //mitk::ProgressBar::GetInstance()->Progress();
    }

};


class RegistrationWorker : public QObject
{
    Q_OBJECT

public:

    RegistrationWorker();
    ~RegistrationWorker() {};

    // image typedefs
    using PixelType = float;
    using FixedImageType = itk::Image<PixelType, 3>;
    using MovingImageType = itk::Image<PixelType, 3>;

    // typedefs
    using InternalPixelType = float;
    using InternalImageType = itk::Image<InternalPixelType, 3>;

    using TransformType = itk::VersorRigid3DTransform< double >;
    using OptimizerType = itk::VersorRigid3DTransformOptimizer;
    using InterpolatorType = itk::LinearInterpolateImageFunction<InternalImageType, double>;
    using RegistrationType = itk::ImageRegistrationMethod<InternalImageType, InternalImageType>;
    using MIMetricType = itk::MutualInformationImageToImageMetric<InternalImageType, InternalImageType>;
    using MSMetricType = itk::MutualInformationImageToImageMetric<InternalImageType, InternalImageType>;
    using FixedNormalizeFilterType = itk::NormalizeImageFilter<FixedImageType, InternalImageType>;
    using MovingNormalizeFilterType = itk::NormalizeImageFilter<MovingImageType, InternalImageType>;
    using GaussianFilterType = itk::DiscreteGaussianImageFilter<InternalImageType, InternalImageType>;

    // inherited
    void process();

    // inherited signals
    void started(unsigned int workerId);
    void progress(float progress, unsigned int workerId);
    void finished(itk::DataObject::Pointer ptr, unsigned int workerId);
    void loadImage(itk::DataObject::Pointer ptr, unsigned int workerId);

    // callback for the progress command
    void itkProgressCommandCallback(int iter);

    itk::Image<float, 3>::Pointer readImageDICOM(const char* DICOMImagePath);

    void readFixedImageDICOM(const char* DICOMImagePath);
    void readMovingImageDICOM(const char* DICOMImagePath);
    RegistrationWorker::FixedImageType::Pointer getRegistrationResult();
    FixedImageType::Pointer imageResample(FixedImageType::Pointer& originalImage);

    // setters
    void setFixedImage(FixedImageType::Pointer img) {
        m_fixedImage = img;
    }

    void setMovingImage(MovingImageType::Pointer img) {
        m_movingImage = img;
    }

    void setMetricsType(bool metricsType) {
        m_metricsType = metricsType;
    }

    void setIteration(int i) {
        m_iteration = i;
    }

    void setMaximumStepLength(double d) {
        m_maximumStepLength = d;
    }

    void setMinimumStepLength(double d) {
        m_minimumStepLength = d;
    }

    void setRelaxationFactor(double d) {
        m_relaxationFactor = d;
    }

    void setNumberOfSamples(int i) {
        m_numberOfSamples = i;
    }

    void setDefualtPixelValue(double d) {
        m_defaultPixelValue = d;
    }

    unsigned int id;

private:

    void preparePipeline();
    void showImageInfor();

    FixedImageType::Pointer  m_fixedImage;
    MovingImageType::Pointer m_movingImage;
    FixedImageType::Pointer  m_output;

    TransformType::Pointer     m_transform;
    OptimizerType::Pointer     m_optimizer;
    InterpolatorType::Pointer  m_interpolator;
    RegistrationType::Pointer  m_registration;
    MIMetricType::Pointer        m_metric;

    FixedNormalizeFilterType::Pointer  m_fixedNormalizer;
    MovingNormalizeFilterType::Pointer m_movingNormalizer;
    GaussianFilterType::Pointer        m_fixedSmoother;
    GaussianFilterType::Pointer        m_movingSmoother;

    // parameters
    bool   m_metricsType;
    int    m_iteration;
    double m_maximumStepLength;
    double m_minimumStepLength;
    double m_relaxationFactor;
    unsigned int m_numberOfSamples;
    double m_defaultPixelValue;
};

#endif // __RegistrationWorker_h__

