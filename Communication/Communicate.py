import serial

ser = serial.Serial('/dev/ttyACM0', 9600) # 根据实际情况修改串口号和波特率

while True:
    data = ser.readline().decode().strip() # 读取串口数据
    print(data) # 打印数据
