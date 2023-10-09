import cv2
import os
from pathlib import Path
import numpy as np
import sys

def read_rgb(rgb_path):
    rgb_data = np.fromfile(rgb_path, np.uint8)
    unit_len_square = int(len(rgb_data)  / 3 / 16 / 9 + 0.5)
    unit_len = int(unit_len_square ** 0.5 + 0.5)
    height = unit_len * 9
    width = unit_len * 16
    rgb_img = rgb_data.reshape((height, width, 3))
    cv2.imwrite(rgb_path + "_rgb.png", rgb_img[:, :, ::-1])


def main():
    if len(sys.argv) < 2:
        print(f"usage: {sys.argv[0]} <rgb_path>")
        return
    read_rgb(sys.argv[1])

if __name__ == "__main__":
    main()