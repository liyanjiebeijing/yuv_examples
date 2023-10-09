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

    std::vector<unsigned char> yuv_data(fileSize, 0);
    infile.read((char*)yuv_data.data(), fileSize);

    //获取图像的宽高（假设宽高比是16:9）
    double square_value = fileSize / 1.5 / 16 / 9;
    int unit_len = int(sqrt(double(square_value)) + 0.5);
    int img_h = unit_len * 9;
    int img_w = unit_len * 16;

    //my yuv to RGB
    cv::Mat my_bgr_img = yuv420_planar2bgr(yuv_data, img_h, img_w);

    //save result
    cv::imwrite((yuv_img_path + "_my_yuv_planar2bgr.png").c_str(), my_bgr_img);

    return 0;
}