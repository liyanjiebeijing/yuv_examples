import cv2
import os
from pathlib import Path
import numpy as np

def read_yuv_planar(yuv_dir):
    for name in os.listdir(yuv_dir):
        if Path(name).suffix != ".bin": continue
        yuv_path = os.path.join(yuv_dir, Path(name).name)
        yuv_data = np.fromfile(yuv_path, np.uint8)
        unit_len_square = int(len(yuv_data)  / 1.5 / 16 / 9 + 0.5)
        unit_len = int(unit_len_square ** 0.5 + 0.5)
        height = unit_len * 9
        width = unit_len * 16
        y_data = yuv_data[:height * width].reshape((height, width))
        cv2.imwrite(yuv_path + "_y.png", y_data)

if __name__ == "__main__":
    read_yuv_planar("./yuv422_planar_img")

