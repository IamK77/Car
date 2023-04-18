import serial
import time


data={
    0: b'\xFF\x09\x00\x00\x00',
    1: b'\xFF\x09\x00\x01\x00',
    2: b'\xFF\x09\x00\x02\x00',
    3: b'\xFF\x09\x00\x03\x00',
    4: b'\xFF\x09\x00\x04\x00',
    5: b'\xFF\x09\x00\x05\x00',
    6: b'\xFF\x09\x00\x06\x00',
    7: b'\xFF\x09\x00\x07\x00',
    8: b'\xFF\x09\x00\x08\x00',
    9: b'\xFF\x09\x00\x09\x00',
    10: b'\xFF\x09\x00\x0a\x00',
    11: b'\xFF\x09\x00\x0b\x00',
    12: b'\xFF\x09\x00\x0c\x00',
    13: b'\xFF\x09\x00\x0d\x00',
    14: b'\xFF\x09\x00\x0e\x00',
    15: b'\xFF\x09\x00\x0f\x00',
}
def send_data(var: list[list]):
    ser = serial.Serial("/dev/ttyUSB0", baudrate=9600, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)
    if var[0] == 1:
    ser.write(data[11])
    time.sleep(0.3)
    ser.write(data[0])
    time.sleep(0.3)
    ser.write(data[1])
    time.sleep(0.3)
    ser.write(data[2])
    time.sleep(0.3)
    ser.close()



