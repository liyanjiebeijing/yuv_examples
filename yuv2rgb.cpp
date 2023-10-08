#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <opencv2/opencv.hpp>
using namespace std;


inline double clip(double v, double min, double max){
    double ret = v;
    ret = ret < min ? min:ret;
    ret = ret > max ? max:ret;
    return ret;
}

inline void yuv2rgb(unsigned char y, unsigned char u, unsigned char v,
                    unsigned char& R, unsigned char& G, unsigned char& B)
{
    double a = y + 1.402 * (v - 128.0);
    double b = y - 0.344 * (u - 128.0) - 0.714 * (v - 128.0);
    double c = y + 1.772 * (u - 128.0);

    double min = 0;
    double max = 255;
    R = (unsigned char)clip(a, min, max);
    G = (unsigned char)clip(b, min, max);
    B = (unsigned char)clip(c, min, max);
}

cv::Mat yuv2bgr(const cv::Mat& yuv_img){
    // 保存 YUV422(UYVY) 格式图像
    cv::Mat bgr_img(yuv_img.rows, yuv_img.cols, CV_8UC3);
    unsigned char* yuv422_data = (unsigned char*)yuv_img.data;
    unsigned char* bgr_data = (unsigned char*)bgr_img.data;

    for (int i = 0; i < yuv_img.rows; i++) {
        for (int j = 0; j < yuv_img.cols; j += 2) {
            unsigned char u  = yuv422_data[0];
            unsigned char y1 = yuv422_data[1];
            unsigned char v  = yuv422_data[2];
            unsigned char y2 = yuv422_data[3];
            yuv2rgb(y1, u, v, bgr_data[2], bgr_data[1], bgr_data[0]);
            yuv2rgb(y2, u, v, bgr_data[5], bgr_data[4], bgr_data[3]);

            bgr_data += 6;
            yuv422_data += 4;
        }
    }

    return bgr_img;
}


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