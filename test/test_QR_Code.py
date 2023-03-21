import cv2
# import numpy as np
from pyzbar.pyzbar import decode

# 加载图像
img = cv2.imread('test.png', 0)

# 解码二维码
decoded = decode(img)

# 打印结果
print(decoded[0].data.decode('utf-8'))