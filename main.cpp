#include <iostream>
#include <fstream>
#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace std;


int main(int argc, char* argv[]) {
    if(argc < 2){
        printf("usage: %s <img_path>\n", argv[0]);
        return 0;
    }

    std::string origin_img_path = argv[1];

    // 读取 RGB 图像
    cv::Mat rgbImage = cv::imread(origin_img_path);
    if (rgbImage.empty()) {
        std::cerr << "Could not read the RGB image." << std::endl;
        return -1;
    }

    //RGB 转YUV
    cv::Mat yuv;
    cv::cvtColor(rgbImage, yuv, cv::COLOR_BGR2YUV);

    // 创建 YUV422(UYVY) 图像
    cv::Mat yuv_422(rgbImage.rows, rgbImage.cols, CV_8UC2);

    // 保存 YUV422(UYVY) 格式图像
    unsigned char* yuv_data = (unsigned char*)yuv.data;
    unsigned char* yuv422_data = (unsigned char*)yuv_422.data;
    for (int i = 0; i < rgbImage.rows; i++) {
        for (int j = 0; j < rgbImage.cols; j += 2) {
            unsigned char y1 = yuv_data[0];
            unsigned char u1 = yuv_data[1];
            unsigned char v1 = yuv_data[2];
            unsigned char y2 = yuv_data[3];
            unsigned char u2 = yuv_data[4];
            unsigned char v2 = yuv_data[5];
            yuv422_data[0] = u2;
            yuv422_data[1] = y1;
            yuv422_data[2] = v1;
            yuv422_data[3] = y2;
            yuv_data += 6;
            yuv422_data += 4;
        }
    }

    //save origin image
    cv::imwrite("rgb_image.png", rgbImage);

    //save yuv 422 image
    ofstream outfile("yuv422.bin", std::ios::binary);
    if(!outfile){
        std::cerr<<"failed to write yuv422 image"<<endl;
        return -1;
    }
    outfile.write((char*)yuv_422.data, rgbImage.rows * rgbImage.cols * 2);

    //save rgb recover image
    cv::Mat rbg_recover;
    cv::cvtColor(yuv_422, rbg_recover, cv::COLOR_YUV2BGR_Y422);
    cv::imwrite("rbg_recover.png", rbg_recover);

    return 0;
}