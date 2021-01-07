#include <itkImage.h>
#include <itkCurvatureFlowImageFilter.h>
#include <itkConnectedThresholdImageFilter.h>

#include "segmentation_worker.h"

SegmentationWorker::SegmentationWorker()
    : image_(nullptr),
    result_image_(nullptr),
    smoothing_filter_(nullptr),
    regiongrow_filter_(nullptr),
    thresholder_(nullptr),
    lower_value_(-1000), upper_value_(-400)
{
}

SegmentationWorker::~SegmentationWorker()
{
}


void SegmentationWorker::set_input_image(ImageType::Pointer img)
{
	image_ = img;
}

void SegmentationWorker::set_seeds(std::vector<IndexType> seeds)
{
    seeds_ = seeds;
}

void SegmentationWorker::update()
{
    if (image_ == nullptr)
    {
        return;
    }

	smoothing_filter_ = SegmentationWorker::CurvatureFlowFilter::New();
    smoothing_filter_->SetInput(image_);
    smoothing_filter_->SetNumberOfIterations(4);
    smoothing_filter_->SetTimeStep(0.0625);

    regiongrow_filter_ = SegmentationWorker::RegionGrowingFilterType::New();
    regiongrow_filter_->SetInput(smoothing_filter_->GetOutput());
    regiongrow_filter_->SetLower(lower_value_);
    regiongrow_filter_->SetUpper(upper_value_);


    for (auto seed : seeds_)
    {
        regiongrow_filter_->AddSeed(seed);
    }

    regiongrow_filter_->Update();

    thresholder_ = ThresholdingFilterType::New();
    thresholder_->SetLowerThreshold(0.5);
    thresholder_->SetUpperThreshold(1.5);
    thresholder_->SetOutsideValue(0);
    thresholder_->SetInsideValue(255);
    thresholder_->SetInput(regiongrow_filter_->GetOutput());
    thresholder_->Update();
    
    result_image_ = thresholder_->GetOutput();

}

SegmentationWorker::ImageType::Pointer SegmentationWorker::get_output_image()
{
	return result_image_;
}
