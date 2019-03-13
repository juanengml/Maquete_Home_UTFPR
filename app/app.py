import serial
import time


ser = serial.Serial("/dev/ttyUSB0",9600)
time.sleep(1)

while True:
	ser.write("l")
	time.sleep(1)
	ser.write("d")
	time.sleep(1)
