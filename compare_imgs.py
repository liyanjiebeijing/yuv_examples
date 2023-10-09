from compare_utils import calculate_mae, calculate_max_error, calculate_mse, calculate_cosine_difference
import numpy as np
import cv2
import os
import sys
from pathlib import Path

def add_text(image, text):
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_scale = 1
    font_color = (255, 255, 255)  # 白色
    font_thickness = 2

    # 获取文本的大小（用于确定文本放置位置）
    text_size, _ = cv2.getTextSize(text, font, font_scale, font_thickness)
    text_x = 0
    text_y = image.shape[0] - text_size[1]

    # 在图像上添加文本
    cv2.putText(image, text, (text_x, text_y), font, font_scale, font_color, font_thickness)


def main():
    if len(sys.argv) < 4:
        print(f"usage: {sys.argv[0]} <first_path> <second_path> <save_path>")
        return

    # first_path = "./yuv422_img/1693829987930866_front_wide_camera.png.bin_my_yuv2bgr.png"
    # second_path = "./yuv422_img/1693829987930866_front_wide_camera.png.bin_opencv_yuv2bgr.png"
    first_path = sys.argv[1]
    second_path = sys.argv[2]
    compare_path = sys.argv[3]

    img_first = cv2.imread(first_path)
    img_second = cv2.imread(second_path)

    data_first = np.copy(img_first).astype(np.float32)
    data_second = np.copy(img_second).astype(np.float32)
    diff = np.abs(data_first - data_second) * 10
    diff_img = np.clip(diff, 0, 255).astype(np.uint8)

    #calculate similarity
    data_first = data_first.reshape(-1)
    data_second = data_second.reshape(-1)
    mae = calculate_mae(data_first, data_second)
    max_error = calculate_max_error(data_first, data_second)
    mse = calculate_mse(data_first, data_second)
    cosine_similarity = calculate_cosine_difference(data_first, data_second)
    print(f"MAX_ERROR={max_error:.10f}, MAE={mae:.10f}, " +
        f"MSE={mse:.10f}, cosine_similarity={cosine_similarity:.10f}")

    dst_img_h = 576
    dst_img_w = 1024
    img_first = cv2.resize(img_first, (dst_img_w, dst_img_h))
    img_second = cv2.resize(img_second, (dst_img_w, dst_img_h))
    diff_img = cv2.resize(diff_img, (dst_img_w, dst_img_h))
    # add_text(img_first, "opencv_yuv2bgr")
    # add_text(img_second, "my_yuv2bgr")
    add_text(diff_img, "diff * 10")

    cv2.imwrite(compare_path, np.hstack((img_first, img_second, diff_img)))
    print(f"save compare img to {compare_path}")


if __name__ == "__main__":
    main()