#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "my_yuv.h"
using namespace std;


int main(int argc, char* argv[]) {
    if(argc < 2){
        printf("usage: %s <rgb_img_path>\n", argv[0]);
        return 0;
    }

    std::string rgb_img_path = argv[1];
    cv::Mat origin_bgr_img = cv::imread(rgb_img_path);
    if(!origin_bgr_img.data){
        printf("failed to read img: %s\n", rgb_img_path.c_str());
        return -1;
    }

    //my RGB to YUV and reverse
    cv::Mat yuv_img = bgr2yuv(origin_bgr_img);
    cv::Mat bgr_img = yuv2bgr(yuv_img);
    cv::imwrite((rgb_img_path + "_my_recover.png").c_str(), bgr_img);

    //opencv RGB to YUV and reverse
    yuv_img = opencv_bgr2yuv(origin_bgr_img);
    bgr_img = opencv_yuv2bgr(yuv_img);
    cv::imwrite((rgb_img_path + "_opencv_recover.png").c_str(), bgr_img);

    return 0;
}