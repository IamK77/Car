import cv2
# import numpy as np
from pyzbar.pyzbar import decode

def decode(file_path) -> None:
    img = cv2.imread(filename=file_path, flags=0)
    decoded = decode(img)
    print(decoded[0].data.decode('utf-8'))

if __name__ == '__main__':
    decode('test.png')