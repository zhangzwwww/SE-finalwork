#ifndef STRUCT_DEFINE_H
#define STRUCT_DEFINE_H

#include <string>

using std::string;

// struct define of algorithm parameters

enum RegistrationType {

};

enum MetricsType {

};

struct RegistrationParams {
    string fixed_img_name;
    string moving_img_name;
    RegistrationType registration_type;
    MetricsType metrics_type;
};

struct AlgorithmParams {
    int iteration_nums;
    double max_step;
    double min_step;
    double relaxation_factor;
    int sample_nums;
    int default_pixel_val;
};

struct FusionParams {
    string img0;
    string img1;
    double opacity;
    string fused_img_name;
};

struct ProcessParams {
    string img_name;
    int isosurface_value;
    string surface_name;
};

struct MedianFilterParams {
//    kernel size
    int x;
    int y;
    int z;
};

struct GaussianKernelParams {
    double std_deviation;
    double radius;
};

struct PolygonSmoothingParams {
    int iteration_nums;
    double relaxation_factor;
};

struct SmoothingParams {
    int smooth_type;
    int kernel_size;
    double sigma_x; // specially for gaussian blur
};

struct EdgeDetectParams {
    double threshold_1;
    double threshold_2;
};

struct ThresholdingParams {
    double lower_value;
    double upper_value;
};

#endif // STRUCT_DEFINE_H
