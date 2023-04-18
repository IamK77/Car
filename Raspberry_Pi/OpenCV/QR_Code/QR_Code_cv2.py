import cv2
from pyzbar import pyzbar

def pyzber_decode(image):
    text = pyzbar.decode(image=image)
    for texts in text:
        textdate = texts.data.decode('utf-8')
        # (x, y, w, h) = texts.rect  # 获取二维码的外接矩形顶点坐标
        print('识别内容:' + textdate)
    return None

def cv2_decode(image):
    qr_detector = cv2.QRCodeDetector()
    data, bbox, straight_qrcode = qr_detector.detectAndDecode(image)
    if data:
        print("Decoded Data : {}".format(data))
        return bbox

# 二维码动态识别
camera = cv2.VideoCapture(1)
camera.set(1080, 1080)  # 设置分辨率
camera.set(1920, 1920)
while True:
    (grabbed, frame) = camera.read()  
    # 获取画面中心点
    h1, w1 = frame.shape[0], frame.shape[1]

    dst = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)  # 灰度化
    # dst = cv2.cvtColor(frame, cv2.IMREAD_COLOR)
    # dst = cv2.medianBlur(dst, 5)  # 中值滤波
    # dst = cv2.GaussianBlur(dst, (5, 5), 0)  # 高斯滤波
    # dst = cv2.Canny(dst, 100, 200)  # 边缘检测
    # dst = cv2.cvtColor(dst, cv2.COLOR_BGR2RGB)  # 转换颜色空间
    # dst = cv2.threshold(dst, 127, 255, cv2.THRESH_BINARY)[1]  # 二值化
    # dst = cv2.bitwise_not(dst)  # 取反
    # pyzber_decode(dst)
    bbox = cv2_decode(dst)
    if bbox is not None:    # 画出二维码的外接矩形
        cv2.rectangle(dst, (int(bbox[0][0][0]), int(bbox[0][0][1])), (int(bbox[0][2][0]), int(bbox[0][2][1])), (0, 255, 0), 2)


    cv2.imshow('dst', dst)
    key = cv2.waitKey(1)
    if key == ord("q"):
        break
    # if cv2.waitKey(1) & 0xFF == ord('q'):  # 按q保存一张图片
    #     cv2.imwrite("./frame.jpg", frame)
    #     break
camera.release()
cv2.destroyAllWindows()
