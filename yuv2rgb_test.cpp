#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "my_yuv.h"
using namespace std;


int main(int argc, char* argv[]) {
    if(argc < 2){
        printf("usage: %s <yuv_img_path>\n", argv[0]);
        return 0;
    }

    // std::string origin_img_path = argv[1];
    std::string yuv_img_path = argv[1];

    // 读取 YUV 图像
    std::ifstream infile(yuv_img_path.c_str(), std::ios::binary);
    if(!infile.is_open()){
        printf("failed to open: %s\n", yuv_img_path.c_str());
        return -1;
    }

    infile.seekg(0, std::ios::end);
    int fileSize = infile.tellg();
    if(fileSize <= -1){
        std::cerr << "Failed to get file size." << std::endl;
        return -1;
    }
    infile.seekg(0, std::ios::beg);

    std::vector<char> yuv_data(fileSize, 0);
    infile.read(yuv_data.data(), fileSize);

    //获取图像的宽高（假设宽高比是16:9）
    int square_value = fileSize / 2 / 16 / 9;
    int unit_len = int(sqrt(double(square_value)) + 0.5);
    int img_h = unit_len * 9;
    int img_w = unit_len * 16;

    //opencv yuv 转RGB
    cv::Mat yuv_img(img_h, img_w, CV_8UC2, yuv_data.data());
    cv::Mat bgr_img(img_h, img_w, CV_8UC3);
    cv::cvtColor(yuv_img, bgr_img, cv::COLOR_YUV2BGR_Y422);

    //my yuv to RGB
    cv::Mat my_bgr_img = yuv2bgr(yuv_img);

    //save result
    cv::imwrite((yuv_img_path + "_opencv_yuv2bgr.png").c_str(), bgr_img);
    cv::imwrite((yuv_img_path + "_my_yuv2bgr.png").c_str(), my_bgr_img);

    return 0;
}