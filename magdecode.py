import serial
import struct

ser = serial.Serial('/dev/ttyACM1', 115200)
xs = []
ys = []

ser.reset_input_buffer()

while True:
    while(ser.inWaiting() >= 6):
        magic_byte = struct.unpack('B', ser.read(1))[0]
        if magic_byte != 0xbe:
            continue
        
        status = struct.unpack('B', ser.read(1))[0]
        current = struct.unpack('<f', ser.read(4))[0]

        print("Status: ", status, " Value: ", current)

ser.close()