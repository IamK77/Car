# coding:utf-8

import serial.tools.list_ports
import time
import sys

serialFd = serial.Serial("COM7", 9600, timeout=60)
var = "123+456"
while 1:
        serialFd.write(var.encode())
        time.sleep(5)
# serialFd.write(var.encode())