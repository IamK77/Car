# coding:utf-8

import serial.tools.list_ports
import time
import sys

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
        serialFd = serial.Serial("/dev/ttyUSB0", 9600, timeout=60)
    except serial.SerialException as e:
        print(serialName)
        print(e)
        sys.exit(0)
    return serialFd   

serialFd = serial_ports()
var = "q123+456"
while 1:
        serialFd.write(var.encode())
        time.sleep(5)
