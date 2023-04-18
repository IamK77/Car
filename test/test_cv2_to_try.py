import cv2

img = cv2.imread('test2.jpg')
img = cv2.cvtColor(img, cv2.COLOR_RGBA2RGB) # 

cv2.imshow("img", img) # 让图片长时间停留
cv2.imwrite("./frame.jpg", img)