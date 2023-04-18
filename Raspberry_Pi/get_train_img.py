import cv2


camera = cv2.VideoCapture(0)
camera.set(1080, 1080)  # 设置分辨率
camera.set(1920, 1920)

red = 0
green = 0
blue = 0


while True:
    (grabbed, frame) = camera.read()  
    # 获取画面中心点
    h1, w1 = frame.shape[0], frame.shape[1]
    dst = cv2.cvtColor(frame, cv2.COLOR_RGBA2RGB)
    cv2.imshow('dst', dst)
    # cmd = input("请输入指令")
    if cv2.waitKey(1) & 0xFF == ord('r'):
        print(f'okr{red}')
        cv2.imwrite(f"D:/Git_Code/Car/Raspberry_Pi/red/{red}.jpg", dst)
        red += 1
    elif cv2.waitKey(1) & 0xFF == ord('g'):
        cv2.imwrite(f"D:/Git_Code/Car/Raspberry_Pi/green/{green}.jpg", frame)
        print(f'okg{green}')
        green += 1
    elif cv2.waitKey(1) & 0xFF == ord('b'):
        cv2.imwrite(f"D:/Git_Code/Car/Raspberry_Pi/blue/{blue}.jpg", frame)
        print(f'okb{blue}')
        blue += 1
    elif cv2.waitKey(1) & 0xFF == ord('q'):
        break

camera.release()
cv2.destroyAllWindows()