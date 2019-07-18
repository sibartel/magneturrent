import serial
import struct
import sys

if len(sys.argv) != 2:
    print("Usage: ", sys.argv[0], " <serial device>")
    sys.exit()

ser = serial.Serial(sys.argv[1], 115200)
xs = []
ys = []

ser.reset_input_buffer()

while True:
    while(ser.inWaiting() >= 6):
        magic_byte = struct.unpack('B', ser.read(1))[0]
        if magic_byte != 0xbe:
            continue
        
        status = struct.unpack('B', ser.read(1))[0]
        timestamp = struct.unpack('<I', ser.read(4))[0]
        current = struct.unpack('<f', ser.read(4))[0]

        print("%d;%d;%f" % (status, timestamp, current))

ser.close()