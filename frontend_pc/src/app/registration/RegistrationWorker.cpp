#include <iostream>

#include <QMessageBox>
#include <thread>

#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"

#include "RegistrationWorker.h"


RegistrationWorker::RegistrationWorker()
	: id(002), m_metricsType(0), m_iteration(50), m_maximumStepLength(4), m_minimumStepLength(0.00001)
	, m_relaxationFactor(0.80), m_numberOfSamples(2000), m_defaultPixelValue(-1000)
	, m_fixedImage(nullptr), m_movingImage(nullptr)
{
	m_transform = TransformType::New();
	m_optimizer = OptimizerType::New();
	m_interpolator = InterpolatorType::New();
	m_registration = RegistrationType::New();

	m_registration->SetOptimizer(m_optimizer);
	m_registration->SetTransform(m_transform);
	m_registration->SetInterpolator(m_interpolator);

	m_fixedNormalizer = FixedNormalizeFilterType::New();
	m_movingNormalizer = MovingNormalizeFilterType::New();

	m_fixedSmoother = GaussianFilterType::New();
	m_movingSmoother = GaussianFilterType::New();

	m_fixedSmoother->SetVariance(2.0);
	m_movingSmoother->SetVariance(2.0);

	using OptimizerScalesType = OptimizerType::ScalesType;
	OptimizerScalesType optimizerScales(m_transform->GetNumberOfParameters());
	const double translationScale = 1.0 / 1000.0;
	optimizerScales[0] = 1.0;
	optimizerScales[1] = 1.0;
	optimizerScales[2] = 1.0;
	optimizerScales[3] = translationScale;
	optimizerScales[4] = translationScale;
	optimizerScales[5] = translationScale;

	m_optimizer->SetScales(optimizerScales);

}

void RegistrationWorker::preparePipeline() 
{

	// ****************************************************************************
	// PART 1: 为配准方法参数赋值
	// ****************************************************************************

	m_metric = MIMetricType::New();
	m_metric->SetFixedImageStandardDeviation(0.4);
	m_metric->SetMovingImageStandardDeviation(0.4);
	m_optimizer->MaximizeOn();

	m_registration->SetMetric(m_metric);


	m_optimizer->SetNumberOfIterations(m_iteration);
	m_optimizer->SetMaximumStepLength(m_maximumStepLength);
	m_optimizer->SetMinimumStepLength(m_minimumStepLength);

	// 方法默认值为0.5，但这会对噪声太过敏感，有时噪声的存在会使得学习率提前减小，因此此项目默认设置为0.8
	m_optimizer->SetRelaxationFactor(m_relaxationFactor);

	const auto numberOfSamples = static_cast<unsigned int>(m_numberOfSamples);
	/* numberOfSamples与图像尺寸大小有关，图像尺寸越大设置的数量应该越大；
	   若重采样为256*256的话只需设置2000即可获得较好的性能与速度，若为512*512尺寸需要设置3000*/

	m_metric->SetNumberOfSpatialSamples(numberOfSamples);
	// For consistent results when regression testing.
	m_metric->ReinitializeSeed(1000);

	// 将图像重采样为256*256，加快配准速度
	FixedImageType::Pointer resampledImage = imageResample(m_fixedImage);

	// 先标准化再去噪
	m_fixedNormalizer->SetInput(resampledImage);
	m_movingNormalizer->SetInput(m_movingImage);

	m_fixedSmoother->SetInput(m_fixedNormalizer->GetOutput());
	m_movingSmoother->SetInput(m_movingNormalizer->GetOutput());


	// ****************************************************************************
	// PART 2: 刚性变化 m_transform 参数初始化
	// ****************************************************************************

	// 3D刚性变换中心初始化
	using TransformInitializerType = itk::CenteredTransformInitializer<TransformType, FixedImageType, MovingImageType>;
	TransformInitializerType::Pointer initializer = TransformInitializerType::New();

	initializer->SetTransform(m_transform);
	initializer->SetFixedImage(m_fixedSmoother->GetOutput());
	initializer->SetMovingImage(m_movingSmoother->GetOutput());
	//initializer->MomentsOn(); // 设置移动中心为质心， 会提示说像素超出范围
	// 设置质心通常不适用于多模态配准
	initializer->GeometryOn(); // Fixed Image的中心坐标 
	initializer->InitializeTransform();

	using VersorType = TransformType::VersorType;
	using VectorType = VersorType::VectorType;
	VersorType  rotation;
	VectorType  axis;
	axis[0] = 0.0;
	axis[1] = 0.0;
	axis[2] = 1.0;
	constexpr double angle = 0;
	rotation.Set(axis, angle);
	m_transform->SetRotation(rotation);


	// ****************************************************************************
	// PART 3: 配准方法 m_registration 参数配置
	// ****************************************************************************

	// 配置参数
	m_registration->SetFixedImage(m_fixedSmoother->GetOutput());
	m_registration->SetMovingImage(m_movingSmoother->GetOutput());

	m_fixedNormalizer->Update();
	FixedImageType::RegionType fixedImageRegion = m_fixedNormalizer->GetOutput()->GetBufferedRegion();
	m_registration->SetFixedImageRegion(fixedImageRegion);

	// transform->GetFixedParameters()、GetParameters()，二者均返回itk::TransformBase::ParametersType类型的对象
	// 待后续优化完成后，再使用registration->GetLastTransformParameters()得到优化后的六维向量
	// 此处只需要设置六维向量，不需要设置旋转中心，因为开始时已设置了m_registration->SetTransform(m_transform);
	// SetInitialTransformParameters只需设置六维向量，SetTransform需要设置完整的变换（6+3）
	m_registration->SetInitialTransformParameters(m_transform->GetParameters());

	// add progress observer
	CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
	//static_cast<CommandIterationUpdate*>(observer.GetPointer())->SetCallbackWorker(this);
	m_optimizer->AddObserver(itk::IterationEvent(), observer);

	// 显示图像信息
	showImageInfor();

}



itk::Image<float, 3>::Pointer RegistrationWorker::readImageDICOM(const char* DICOMImagePath)
{
	using FixedImageReaderType = itk::ImageSeriesReader<FixedImageType>;
	FixedImageReaderType::Pointer  fixedImageReader = FixedImageReaderType::New();

	using ImageIOType = itk::GDCMImageIO;
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	fixedImageReader->SetImageIO(dicomIO);
	//fixedImageReader->ForceOrthogonalDirectionOff(); // properly read CTs with gantry tilt

	using NamesGeneratorType = itk::GDCMSeriesFileNames;
	NamesGeneratorType::Pointer fixedNameGenerator = NamesGeneratorType::New();
	fixedNameGenerator->SetUseSeriesDetails(true);
	fixedNameGenerator->SetDirectory(DICOMImagePath);
	fixedNameGenerator->AddSeriesRestriction("0008|0021"); // tag: series date 保证是同一个病人

	try
	{
		//std::cout << std::endl;

		// UID ( Unique Identifiers ) 唯一标志值，每个病人拥有唯一的标志值
		using SeriesIdContainer = std::vector< std::string >;
		const SeriesIdContainer& fixedSeriesUID = fixedNameGenerator->GetSeriesUIDs();

		//std::cout << "Contains the following DICOM Series: ";
		auto seriesItr = fixedSeriesUID.begin();
		auto seriesEnd = fixedSeriesUID.end();

		if (seriesItr == seriesEnd)
		{

			QMessageBox::warning(nullptr,
				tr("Selection Error"),
				tr("please select the folder containing DICOM files."),
				QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

			return nullptr;
		}


		while (seriesItr != seriesEnd)
		{
			//std::cout << seriesItr->c_str() << std::endl << std::endl << std::endl;
			++seriesItr;
		}

		std::string fixedSeriesIdentifier;
		std::string movingSeriesIdentifier;

		// 每个病人都有其独特的UID
		fixedSeriesIdentifier = fixedSeriesUID.begin()->c_str();

		using FileNamesContainer = std::vector< std::string >;
		FileNamesContainer fixedFileNames;

		fixedFileNames = fixedNameGenerator->GetFileNames(fixedSeriesIdentifier);
		fixedImageReader->SetFileNames(fixedFileNames);

		try
		{
			fixedImageReader->Update();
		}
		catch (itk::ExceptionObject& ex)
		{
			QMessageBox::warning(nullptr,
				tr("Error"),
				tr(ex.GetDescription()),
				QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		}

		return fixedImageReader->GetOutput();

	}
	catch (itk::ExceptionObject& ex)
	{
		QMessageBox::warning(nullptr,
			tr("Error"),
			tr(ex.GetDescription()),
			QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	}

}

void RegistrationWorker::readFixedImageDICOM(const char* DICOMImagePath)
{
	using FixedImageReaderType = itk::ImageSeriesReader<FixedImageType>;
	FixedImageReaderType::Pointer  fixedImageReader = FixedImageReaderType::New();

	using ImageIOType = itk::GDCMImageIO;
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	fixedImageReader->SetImageIO(dicomIO);
	//fixedImageReader->ForceOrthogonalDirectionOff(); // properly read CTs with gantry tilt

	using NamesGeneratorType = itk::GDCMSeriesFileNames;
	NamesGeneratorType::Pointer fixedNameGenerator = NamesGeneratorType::New();
	fixedNameGenerator->SetUseSeriesDetails(true);
	fixedNameGenerator->SetDirectory(DICOMImagePath);
	fixedNameGenerator->AddSeriesRestriction("0008|0021"); // tag: series date 保证是同一个病人

	try
	{
		std::cout << std::endl;

		// UID ( Unique Identifiers ) 唯一标志值，每个病人拥有唯一的标志值
		using SeriesIdContainer = std::vector< std::string >;
		const SeriesIdContainer& fixedSeriesUID = fixedNameGenerator->GetSeriesUIDs();

		std::cout << "Contains the following DICOM Series: ";
		auto seriesItr = fixedSeriesUID.begin();
		auto seriesEnd = fixedSeriesUID.end();

		if (seriesItr == seriesEnd) 
		{

			QMessageBox::warning(nullptr,
				tr("Selection Error"),
				tr("please select the folder containing DICOM files."),
				QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

			return;
		}


		while (seriesItr != seriesEnd)
		{
			std::cout << seriesItr->c_str() << std::endl << std::endl << std::endl;
			++seriesItr;
		}

		std::string fixedSeriesIdentifier;
		std::string movingSeriesIdentifier;

		// 每个病人都有其独特的UID
		fixedSeriesIdentifier = fixedSeriesUID.begin()->c_str();

		using FileNamesContainer = std::vector< std::string >;
		FileNamesContainer fixedFileNames;

		fixedFileNames = fixedNameGenerator->GetFileNames(fixedSeriesIdentifier);
		fixedImageReader->SetFileNames(fixedFileNames);

		try
		{
			fixedImageReader->Update();
		}
		catch (itk::ExceptionObject& ex)
		{
			std::cout << ex << std::endl;
		}

		FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();

	}
	catch (itk::ExceptionObject& ex)
	{
		std::cout << ex << std::endl;
	}

}

void RegistrationWorker::readMovingImageDICOM(const char* DICOMImagePath)
{
	using MovingImageReaderType = itk::ImageSeriesReader<MovingImageType>;
	MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();

	using ImageIOType = itk::GDCMImageIO;
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	movingImageReader->SetImageIO(dicomIO);
	//movingImageReader->ForceOrthogonalDirectionOff(); // properly read CTs with gantry tilt

	using NamesGeneratorType = itk::GDCMSeriesFileNames;
	NamesGeneratorType::Pointer movingNameGenerator = NamesGeneratorType::New();
	movingNameGenerator->SetUseSeriesDetails(true);
	movingNameGenerator->SetDirectory(DICOMImagePath);
	movingNameGenerator->AddSeriesRestriction("0008|0021");

	try
	{
		std::cout << std::endl;

		// UID ( Unique Identifiers ) 唯一标志值，每个病人拥有唯一的标志值
		using SeriesIdContainer = std::vector< std::string >;
		const SeriesIdContainer& movingSeriesUID = movingNameGenerator->GetSeriesUIDs();

		std::string movingSeriesIdentifier;

		// 每个病人都有其独特的UID
		movingSeriesIdentifier = movingSeriesUID.begin()->c_str();

		using FileNamesContainer = std::vector< std::string >;
		FileNamesContainer fixedFileNames;
		FileNamesContainer movingFileNames;

		movingFileNames = movingNameGenerator->GetFileNames(movingSeriesIdentifier);
		movingImageReader->SetFileNames(movingFileNames);

		try
		{
			movingImageReader->Update();
		}
		catch (itk::ExceptionObject& ex)
		{
			std::cout << ex << std::endl;

		}

		MovingImageType::Pointer movingImage = movingImageReader->GetOutput();

	}
	catch (itk::ExceptionObject& ex)
	{
		std::cout << ex << std::endl;
	}

}


RegistrationWorker::FixedImageType::Pointer RegistrationWorker::getRegistrationResult()
{
	try
	{
		std::cout << std::endl <<
			" ----------------- Registration Start ---------------- " << std::endl << std::endl;

		m_registration->Update();

		std::cout << std::endl << "Optimizer stop condition: " << m_registration->GetOptimizer()->GetStopConditionDescription()
			<< std::endl << std::endl;
	}
	catch (itk::ExceptionObject& err)
	{
		std::cout << "ExceptionObject caught !" << std::endl;
		std::cout << err << std::endl;
	}

	using ParametersType = RegistrationType::ParametersType;
	ParametersType finalParameters = m_registration->GetLastTransformParameters();

	double TranslationAlong1 = finalParameters[0];
	double TranslationAlong2 = finalParameters[1];
	double TranslationAlong3 = finalParameters[2];
	double TranslationAlong4 = finalParameters[3];
	double TranslationAlong5 = finalParameters[4];
	double TranslationAlong6 = finalParameters[5];

	unsigned int numberOfIterations = m_optimizer->GetCurrentIteration();

	double bestValue = m_optimizer->GetValue();

	// Print out results
	std::cout << std::endl;
	std::cout << "Transform Result = " << std::endl;
	std::cout << " Translation 1 = " << TranslationAlong1 << std::endl;
	std::cout << " Translation 2 = " << TranslationAlong2 << std::endl;
	std::cout << " Translation 3 = " << TranslationAlong3 << std::endl;
	std::cout << " Translation 4 = " << TranslationAlong4 << std::endl;
	std::cout << " Translation 5 = " << TranslationAlong5 << std::endl;
	std::cout << " Translation 6 = " << TranslationAlong6 << std::endl;
	std::cout << " Iterations    = " << numberOfIterations << std::endl;
	std::cout << " Metric value  = " << bestValue << std::endl;
	std::cout << " Numb. Samples = " << m_numberOfSamples << std::endl;

	TransformType::Pointer finalTransform = TransformType::New();;
	finalTransform->SetParameters(finalParameters);
	finalTransform->SetFixedParameters(m_transform->GetFixedParameters());

	using ResampleFilterType = itk::ResampleImageFilter<MovingImageType, FixedImageType>;
	ResampleFilterType::Pointer resample = ResampleFilterType::New();

	resample->SetTransform(finalTransform);
	//resample->SetTransform(m_registration->GetOutput()->Get());
	resample->SetInput(m_movingImage);
	resample->SetSize(m_fixedImage->GetLargestPossibleRegion().GetSize());
	resample->SetOutputOrigin(m_fixedImage->GetOrigin());
	resample->SetOutputSpacing(m_fixedImage->GetSpacing());
	// 此处的direction可能指的是DICOM中的方向，即图像第一行与第一列与病人标准正交方向的cosine值
	// 是一个六维的向量，如1/0/0/0/0.9862/-0.1650，代表图像行方向为标准x方向，列方向与标准y方向的cosine为0.9862
	resample->SetOutputDirection(m_fixedImage->GetDirection());
	resample->SetDefaultPixelValue(m_defaultPixelValue);

	try
	{
		resample->Update();
		std::cout << std::endl << "Moving Image Resample Update Success" << std::endl;
	}
	catch (itk::ExceptionObject& error)
	{
		std::cerr << "Error: " << error << std::endl;
	}

	FixedImageType::Pointer finishedImage = resample->GetOutput();

	return finishedImage;

}


RegistrationWorker::FixedImageType::Pointer RegistrationWorker::imageResample(FixedImageType::Pointer& originalImage)
{
	typedef itk::ResampleImageFilter<FixedImageType, FixedImageType> ResampleImageFilterType;
	ResampleImageFilterType::Pointer resampleFilter = ResampleImageFilterType::New();

	resampleFilter->SetInput(originalImage);

	using SizeType = FixedImageType::SizeType;
	using PixelType = FixedImageType::PixelType;
	using PoingType = FixedImageType::PointType;
	using IndexType = FixedImageType::IndexType;
	using DirectionType = FixedImageType::DirectionType;
	using SpacingType = FixedImageType::SpacingType;

	SpacingType originspacing = originalImage->GetSpacing();
	SizeType size = originalImage->GetLargestPossibleRegion().GetSize();
	SizeType sizesample;
	sizesample[0] = 256;
	sizesample[1] = 256;
	sizesample[2] = size[2];
	SpacingType spacingresample;
	spacingresample[0] = size[0] * originspacing[0] / sizesample[0];
	spacingresample[1] = size[1] * originspacing[1] / sizesample[1];
	spacingresample[2] = size[2] * originspacing[2] / sizesample[2];

	InterpolatorType::Pointer interpolator = InterpolatorType::New();
	resampleFilter->SetInterpolator(interpolator);
	resampleFilter->SetOutputSpacing(spacingresample);
	resampleFilter->SetSize(sizesample);
	resampleFilter->SetOutputOrigin(originalImage->GetOrigin());
	resampleFilter->SetOutputDirection(originalImage->GetDirection());
	resampleFilter->SetDefaultPixelValue(m_defaultPixelValue);

	try
	{
		resampleFilter->Update();
	}
	catch (itk::ExceptionObject& exp)
	{
		std::cerr << "Resample Error!" << std::endl;
		std::cerr << exp << std::endl;
	}

	return resampleFilter->GetOutput();
}

void RegistrationWorker::showImageInfor()
{

	std::cout << std::endl <<
		" ----------------- Image Information ---------------- " << std::endl;

	// Fixed Image
	std::cout << std::endl << "Fixed Image Information: " << std::endl;
	std::cout << std::endl;

	const FixedImageType::SpacingType& spacing = m_fixedImage->GetSpacing();
	const FixedImageType::PointType& origin = m_fixedImage->GetOrigin();
	const FixedImageType::SizeType& size = m_fixedImage->GetLargestPossibleRegion().GetSize();

	std::cout << " Spacing = " << spacing[0] << " " << spacing[1] << " " << spacing[2] << std::endl;
	std::cout << " Origin = " << origin[0] << " " << origin[1] << " " << origin[2] << std::endl;
	std::cout << " Size = " << size[0] << " " << size[1] << " " << size[2] << std::endl;


	// Moving Image
	std::cout << std::endl << "Moving Image Information: " << std::endl;
	std::cout << std::endl;

	const MovingImageType::SpacingType& spacing1 = m_movingImage->GetSpacing();
	const MovingImageType::PointType& origin1 = m_movingImage->GetOrigin();
	const MovingImageType::SizeType& size1 = m_movingImage->GetLargestPossibleRegion().GetSize();

	std::cout << " Spacing = " << spacing1[0] << " " << spacing1[1] << " " << spacing1[2] << std::endl;
	std::cout << " Origin = " << origin1[0] << " " << origin1[1] << " " << origin1[2] << std::endl;
	std::cout << " Size = " << size1[0] << " " << size1[1] << " " << size1[2] << std::endl;


	// 显示 Fixed Image 旋转中心坐标
	std::cout << std::endl << "Image Center Information: " << std::endl;

	RegistrationType::ParametersType center = m_transform->GetFixedParameters();
	std::cout << std::endl << "Center of Fixed Image is: " << "[" << center[0] << ", "
		<< center[1] << ", " << center[2] << "]" << std::endl;

	FixedImageType::IndexType pixelIndex;
	FixedImageType::PointType point;
	point[0] = center[0];
	point[1] = center[1];
	point[2] = center[2];

	bool isInside = m_fixedSmoother->GetOutput()->TransformPhysicalPointToIndex(point, pixelIndex);
	if (isInside)
	{
		std::cout << "The Center of Fixed Image is: " << "[" << pixelIndex[0] << ", "
			<< pixelIndex[1] << ", " << pixelIndex[2] << "]" << std::endl << std::endl;
	}
	else
	{
		std::cout << "the point is outside of image !" << std::endl << std::endl;
	}


}

