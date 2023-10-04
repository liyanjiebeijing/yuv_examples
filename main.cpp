#include <iostream>
#include <fstream>
#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace std;

inline double clip(double v, double min, double max){
    double ret = v;
    ret = ret < min ? min:ret;
    ret = ret > max ? max:ret;
    return ret;
}

inline void YUVfromRGB(unsigned char R, unsigned char G, unsigned char B,
                unsigned char &Y, unsigned char &U, unsigned char &V)
{
  double a =  0.257 * R + 0.504 * G + 0.098 * B +  16;
  double b = -0.148 * R - 0.291 * G + 0.439 * B + 128;
  double c =  0.439 * R - 0.368 * G - 0.071 * B + 128;

  double min = 0;
  double max = 255;
  Y = (unsigned char)clip(a, 0, 255);
  U = (unsigned char)clip(b, 0, 255);
  V = (unsigned char)clip(c, 0, 255);
}

int main(int argc, char* argv[]) {
    if(argc < 2){
        printf("usage: %s <img_path>\n", argv[0]);
        return 0;
    }

    std::string origin_img_path = argv[1];

    // 读取 RGB 图像
    cv::Mat bgrImage = cv::imread(origin_img_path);
    if (bgrImage.empty()) {
        std::cerr << "Could not read the RGB image." << std::endl;
        return -1;
    }

    //RGB 转YUV
    // cv::Mat yuv;
    // cv::cvtColor(bgrImage, yuv, cv::COLOR_BGR2YUV);

    // 创建 YUV422(UYVY) 图像
    cv::Mat yuv_422(bgrImage.rows, bgrImage.cols, CV_8UC2);

    // 保存 YUV422(UYVY) 格式图像
    unsigned char* bgr_data = (unsigned char*)bgrImage.data;
    unsigned char* yuv422_data = (unsigned char*)yuv_422.data;
    for (int i = 0; i < bgrImage.rows; i++) {
        for (int j = 0; j < bgrImage.cols; j += 2) {
            unsigned char B1 = bgr_data[0];
            unsigned char G1 = bgr_data[1];
            unsigned char R1 = bgr_data[2];
            unsigned char B2 = bgr_data[3];
            unsigned char G2 = bgr_data[4];
            unsigned char R2 = bgr_data[5];

            unsigned char y1 = 0;
            unsigned char u1 = 0;
            unsigned char v1 = 0;
            unsigned char y2 = 0;
            unsigned char u2 = 0;
            unsigned char v2 = 0;

            YUVfromRGB(R1, G1, B1, y1, u1, v1);
            YUVfromRGB(R2, G2, B2, y2, u2, v2);
            yuv422_data[0] = u2;
            yuv422_data[1] = y1;
            yuv422_data[2] = v1;
            yuv422_data[3] = y2;

            bgr_data += 6;
            yuv422_data += 4;
        }
    }

    //save origin image
    cv::imwrite("rgb_image.png", bgrImage);

    //save yuv 422 image
    ofstream outfile("yuv422.bin", std::ios::binary);
    if(!outfile){
        std::cerr<<"failed to write yuv422 image"<<endl;
        return -1;
    }
    outfile.write((char*)yuv_422.data, bgrImage.rows * bgrImage.cols * 2);

    //save rgb recover image
    cv::Mat rbg_recover;
    cv::cvtColor(yuv_422, rbg_recover, cv::COLOR_YUV2BGR_Y422);
    cv::imwrite("rbg_recover.png", rbg_recover);

    return 0;
}