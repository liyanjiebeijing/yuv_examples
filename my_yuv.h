#ifndef __MY_YUV_H__
#define __MY_YUV_H__

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <opencv2/opencv.hpp>

cv::Mat yuv2bgr(const cv::Mat& yuv_img);
cv::Mat yuv422_planar2bgr(const cv::Mat& yuv_img);
cv::Mat yuv420_planar2bgr(const std::vector<unsigned char>& yuv, int img_h, int img_w);
cv::Mat bgr2yuv(const cv::Mat& bgrImage);

cv::Mat opencv_bgr2yuv(const cv::Mat& bgrImage);
cv::Mat opencv_yuv2bgr(const cv::Mat& yuv_img);


#endif