import cv2

import numpy as np

def detect_color(img, color):
    # hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    hsv = cv2.cvtColor(img, cv2.COLOR_RGBA2RGB)
    if color == "red":
        lower_color = np.array([0, 50, 50])
        upper_color = np.array([10, 255, 255])
        color_code = (0, 0, 255)
    elif color == "blue":
        lower_color = np.array([110, 50, 50])
        upper_color = np.array([130, 255, 255])
        color_code = (255, 0, 0)
    elif color == "green":
        lower_color = np.array([50, 50, 50])
        upper_color = np.array([70, 255, 255])
        color_code = (0, 255, 0)
    else:
        print("Invalid color")
        return None
    mask = cv2.inRange(hsv, lower_color, upper_color)
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    for cnt in contours:
        x, y, w, h = cv2.boundingRect(cnt)
        if x > 200 and x < 400 and y > 150 and y < 350:
            cv2.rectangle(img, (x, y), (x + w, y + h), color_code, 2)
    return img

cap = cv2.VideoCapture(1)

while True:
    ret, frame = cap.read()
    if not ret:
        break
    red_mask = detect_color(frame, "red")
    blue_mask = detect_color(frame, "blue")
    green_mask = detect_color(frame, "green")
    cv2.imshow('frame', frame)
    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
