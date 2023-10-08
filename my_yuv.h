#ifndef __MY_YUV_H__
#define __MY_YUV_H__

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <opencv2/opencv.hpp>

cv::Mat yuv2bgr(const cv::Mat& yuv_img);
cv::Mat bgr2yuv(const cv::Mat& bgrImage);

#endif