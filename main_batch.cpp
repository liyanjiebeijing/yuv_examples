#include <iostream>
#include <fstream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <dirent.h>
using namespace std;


int is_file(const char *filename, const std::string& suffix) {
    const char *ext = strrchr(filename, '.');
    if (ext != NULL && strcmp(ext, suffix.c_str()) == 0) {
        return 1;
    }
    return 0;
}

std::vector<std::string> list_dir(const std::string& file_dir, const std::string& suffix) {
    std::vector<std::string> file_names;
    DIR *dir = 0;
    struct dirent *ent = 0;

    if ((dir = opendir(file_dir.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (is_file(ent->d_name, suffix)) {
                file_names.push_back( ent->d_name);
            }
        }
        closedir(dir);
    } else {
        perror("Error opening directory");
        return file_names;
    }

    return file_names;
}


int main(int argc, char* argv[]) {
    if(argc < 3){
        printf("usage: %s <src_dir> <dst_dir>\n", argv[0]);
        return 0;
    }

    std::string origin_img_dir = argv[1];
    std::string dst_yuv_img_dir = argv[2];
    auto file_paths = list_dir(origin_img_dir.c_str(), ".png");

    // 读取 RGB 图像
    for(auto name: file_paths){
        auto origin_img_path = origin_img_dir + "/" + name;
        cv::Mat rgbImage = cv::imread(origin_img_path.c_str());

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
        // cv::imwrite("rgb_image.png", rgbImage);

        //save yuv 422 image
        auto dest_path = dst_yuv_img_dir + "/" + name + ".bin";
        ofstream outfile(dest_path.c_str(), std::ios::binary);
        if(!outfile){
            std::cerr<<"failed to write yuv422 image"<<endl;
            return -1;
        }
        outfile.write((char*)yuv_422.data, rgbImage.rows * rgbImage.cols * 2);
        cout<<"save to "<<dest_path.c_str()<<endl;
    }

    return 0;
}