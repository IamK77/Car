import cv2
import numpy as np
from pyzbar.pyzbar import decode

def decode(file_path) -> None:
    img = cv2.imread(filename=file_path, flags=0)   # 加载图像
    try:
        kernel = np.array([[-1,-1,-1], [-1,9,-1], [-1,-1,-1]])
        sharpened = cv2.filter2D(img, -1, kernel)   # 解码二维码
        decoded = decode(sharpened)
        decoded = decode(img)
        print(decoded[0].data.decode('utf-8'))
        print(1)
    # except Exception as e:
    #     qrCodeDetector = cv2.QRCodeDetector()
    #     data, bbox, straight_qrcode = qrCodeDetector.detectAndDecode(img)
    #     print(2)
    #     print(data)
    except:
        import zxing
        reader = zxing.BarCodeReader()
        barcode = reader.decode(file_path)
        print(3)
        print(barcode.parsed)
    finally:
        print('识别失败')

if __name__ == '__main__':
    decode("D:/Git Code/Car/test/test.jpg")