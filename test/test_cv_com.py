import cv2
import time
from pyzbar import pyzbar
import serial.tools.list_ports
import sys

def pyzber_decode(image):
    text = pyzbar.decode(image=image)
    for texts in text:
        textdate = texts.data.decode('utf-8')
        print('识别内容:' + textdate)
    return None

def cv2_decode(image):
    qr_detector = cv2.QRCodeDetector()
    data, bbox, straight_qrcode = qr_detector.detectAndDecode(image)
    if data:
        return data

# 二维码动态识别
camera = cv2.VideoCapture(0)
camera.set(1080, 1080)  # 设置分辨率
camera.set(1920, 1920)
serialFd = serial.Serial("COM7", 9600, timeout=60)

data_list = []
time_start = time.time()
while True:
    (grabbed, frame) = camera.read()  
    # 获取画面中心点
    h1, w1 = frame.shape[0], frame.shape[1]
    dst = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)  # 灰度化
    
    data = cv2_decode(dst)
    if type(data) == str:
        data_list.append(data)
    # 选出10个数据中最多的那个
    if len(data_list) == 20:
        result = max(data_list, key=data_list.count)
        while 1:
            serialFd.write(result.encode())
            time.sleep(5)
            break
        break

    cv2.imshow('dst', dst)
    if cv2.waitKey(1) & 0xFF == ord('q'):  # 按q保存一张图片
        cv2.imwrite("./frame.jpg", frame)
        break

time_end = time.time()
print('time cost', time_end - time_start, 's')
camera.release()
serialFd.close()
cv2.destroyAllWindows()