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


inline void rgb2yuv(unsigned char R, unsigned char G, unsigned char B,
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



cv::Mat yuv422_planar2bgr(const cv::Mat& yuv_img){
    // 保存 YUV422(UYVY) 格式图像
    cv::Mat bgr_img(yuv_img.rows, yuv_img.cols, CV_8UC3);
    unsigned char* y_data = (unsigned char*)yuv_img.data;
    unsigned char* u_data = (unsigned char*)yuv_img.data + yuv_img.rows * yuv_img.cols;
    unsigned char* v_data = (unsigned char*)yuv_img.data + yuv_img.rows * yuv_img.cols / 2 * 3;
    unsigned char* bgr_data = (unsigned char*)bgr_img.data;

    for (int i = 0; i < yuv_img.rows; i++) {
        for (int j = 0; j < yuv_img.cols; j += 2) {
            unsigned char u  = u_data[0];
            unsigned char y1 = y_data[0];
            unsigned char v  = v_data[0];
            unsigned char y2 = y_data[1];
            yuv2rgb(y1, u, v, bgr_data[2], bgr_data[1], bgr_data[0]);
            yuv2rgb(y2, u, v, bgr_data[5], bgr_data[4], bgr_data[3]);

            bgr_data += 6;
            y_data += 2;
            v_data++;
            u_data++;
        }
    }

    return bgr_img;
}


cv::Mat yuv420_planar2bgr(const std::vector<unsigned char>& yuv, int img_h, int img_w){
    const unsigned char* yuv_data = &yuv[0];
    cv::Mat bgr_img(img_h, img_w, CV_8UC3);
    const unsigned char* u_data = yuv_data + img_h * img_w;
    const unsigned char* v_data = yuv_data + img_h * img_w / 4 * 5;

    for (int i = 0; i < img_h; i += 2) {
        const unsigned char* y_data = yuv_data + img_w * i;
        unsigned char* bgr_data = (unsigned char*)bgr_img.data + img_w * i * 3;

        for (int j = 0; j < img_w; j += 2) {
            unsigned char y1 = y_data[0];
            unsigned char y2 = y_data[1];
            unsigned char y3 = y_data[img_w];
            unsigned char y4 = y_data[img_w + 1];
            unsigned char u  = u_data[0];
            unsigned char v  = v_data[0];

            yuv2rgb(y1, u, v, bgr_data[2], bgr_data[1], bgr_data[0]);
            yuv2rgb(y2, u, v, bgr_data[5], bgr_data[4], bgr_data[3]);
            unsigned char *bgr_data_next_line = bgr_data + 3 * img_w;
            yuv2rgb(y3, u, v, bgr_data_next_line[2], bgr_data_next_line[1], bgr_data_next_line[0]);
            yuv2rgb(y4, u, v, bgr_data_next_line[5], bgr_data_next_line[4], bgr_data_next_line[3]);

            bgr_data += 6;
            y_data += 2;
            v_data++;
            u_data++;
        }
    }

    return bgr_img;
}


cv::Mat bgr2yuv(const cv::Mat& bgrImage){
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

            rgb2yuv(R1, G1, B1, y1, u1, v1);
            rgb2yuv(R2, G2, B2, y2, u2, v2);
            yuv422_data[0] = u1;
            yuv422_data[1] = y1;
            yuv422_data[2] = v1;
            yuv422_data[3] = y2;

            bgr_data += 6;
            yuv422_data += 4;
        }
    }

    return yuv_422;
}



cv::Mat opencv_bgr2yuv(const cv::Mat& bgrImage){
    //RGB 转YUV
    cv::Mat yuv;
    cv::cvtColor(bgrImage, yuv, cv::COLOR_BGR2YUV);

    // 创建 YUV422(UYVY) 图像
    cv::Mat yuv_422(bgrImage.rows, bgrImage.cols, CV_8UC2);

    // 保存 YUV422(UYVY) 格式图像
    unsigned char* yuv_data = (unsigned char*)yuv.data;
    unsigned char* yuv422_data = (unsigned char*)yuv_422.data;
    for (int i = 0; i < bgrImage.rows; i++) {
        for (int j = 0; j < bgrImage.cols; j += 2) {
            unsigned char y1 = yuv_data[0];
            unsigned char u1 = yuv_data[1];
            unsigned char v1 = yuv_data[2];
            unsigned char y2 = yuv_data[3];
            unsigned char u2 = yuv_data[4];
            unsigned char v2 = yuv_data[5];
            yuv422_data[0] = u1;
            yuv422_data[1] = y1;
            yuv422_data[2] = v1;
            yuv422_data[3] = y2;
            yuv_data += 6;
            yuv422_data += 4;
        }
    }

    return yuv_422;
}


cv::Mat opencv_yuv2bgr(const cv::Mat& yuv_img){
    cv::Mat bgr_img;
    cv::cvtColor(yuv_img, bgr_img, cv::COLOR_YUV2BGR_Y422);
    return bgr_img;
}



