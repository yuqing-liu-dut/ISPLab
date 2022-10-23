#pragma once
#include "MyImage.h"

#include <opencv2/opencv.hpp>

cv::Mat toCvMat(ImageRGB& rgb);
cv::Mat dnnExecute(cv::Mat img, const char* model);
