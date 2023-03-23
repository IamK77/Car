import cv2
import numpy as np
from pyzbar.pyzbar import decode

# 加载图像
img = cv2.imread('test.jpg', 0)
kernel = np.array([[-1,-1,-1], [-1,9,-1], [-1,-1,-1]])
sharpened = cv2.filter2D(img, -1, kernel)
# 解码二维码
decoded = decode(sharpened)
qrCodeDetector = cv2.QRCodeDetector()
data, bbox, straight_qrcode = qrCodeDetector.detectAndDecode(img)

print(data)
# 打印结果
print(decoded[0].data.decode('utf-8'))
