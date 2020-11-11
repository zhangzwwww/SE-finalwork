#include "general_processing.h"

//ERROR_CODE Smoothing(const Mat &input_img, Mat &output_img, SmoothingParams &params) {
//    if (input_img.empty()) {
//        return INPUT_IMG_INVALID;
//    }
//    if (params.kernel_size%2 == 0) {
//        return PARAM_INVALID;
//    }
//    switch (params.smooth_type) {
//    case 0:
//        GaussianBlur(input_img, output_img, Size(params.kernel_size, params.kernel_size), params.sigma_x);
//        break;
//    case 1:
//        blur(input_img, output_img, Size(params.kernel_size, params.kernel_size));
//        break;
//    case 2:
//        medianBlur(input_img, output_img, params.kernel_size);
//        break;
//    default:
//        break;
//    }
//    return OK;
//}

//ERROR_CODE EdgeDetect(const Mat &input_img, Mat &output_img, EdgeDetectParams &params) {
//    if (input_img.empty()) {
//        return INPUT_IMG_INVALID;
//    }
//    Canny(input_img, output_img, params.threshold_1, params.threshold_2);
//    return OK;
//}

//ERROR_CODE Thresholding(const Mat &input_img, Mat &output_img, ThresholdingParams &params) {
//    if (input_img.empty()) {
//        return INPUT_IMG_INVALID;
//    }
//    if (params.thresh_type >= (int)local_thresh_type.size() || params.thresh_type < 0) {
//        return PARAM_INVALID;
//    }
//    params.thresh_type = local_thresh_type[(unsigned long)params.thresh_type];
//    threshold(input_img, output_img, params.thresh_value, params.max_value, params.thresh_type);
//    return OK;
//}
