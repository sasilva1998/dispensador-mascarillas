import serial

ser = serial.Serial('/dev/tnt1', 9600)
while True:
    data=ser.read()
    print(hex(ord(data)))