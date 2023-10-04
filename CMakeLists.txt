cmake_minimum_required(VERSION 3.18)
project(rgb_to_yuv422)

set(OpenCV_DIR "/build_env/x86/thirdparty/opencv/lib/cmake/opencv4/")

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)

# 查找OpenCV库
find_package(OpenCV REQUIRED)

# 指定可执行文件的名称和源文件
add_executable(rgb_to_yuv422 main.cpp)
add_executable(batch_rgb_to_yuv422 main_batch.cpp)


# 链接OpenCV库
target_link_libraries(rgb_to_yuv422 ${OpenCV_LIBS})
target_link_libraries(batch_rgb_to_yuv422 ${OpenCV_LIBS})