# coding:utf-8

import serial.tools.list_ports
import time
import sys


# num = 12345
# # 把它转换成两个字节的数组
# buf = [num >> 8 & 255, num & 255]
# print(buf)
def serial_ports():
    plist = list(serial.tools.list_ports.comports())
    if len(plist) <= 0:
        print("没有发现端口!")
        sys.exit(0)
    for com in plist:
        print(com)
        print("可用端口名>>>", com[0])
    i = input("输入端口号：")
    try:
        plist_port = list(plist[int(i)-1])
        serialName = plist_port[0]
        print("使用用端口名>>>", serialName)
        serialFd = serial.Serial(serialName, 9600, timeout=60)
    except serial.SerialException as e:
        print(serialName)
        print(e)
        sys.exit(0)
    return serialFd   

serialFd = serial_ports()
var = "q123+456"
while 1:
        serialFd.write(var.encode())
        var = ""
        # speed = input("输入速度：")
        # 假设你要发送的整数是12345
        # num = 12345
        # # 把它转换成两个字节的数组
        # buf = [num >> 8 & 255, num & 255]
        # # 发送数组
        # serialFd.write(buf, 2)
        # # serialFd.write("o".encode())    # 给出写入int类型数据的代码   
        time.sleep(5)
        
