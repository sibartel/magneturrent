import serial


ser = serial.Serial('/dev/ttyACM2', 115200)
xs = []
ys = []

ser.reset_input_buffer()

while True:
    data_raw = ser.read(4)
    print(int.from_bytes(data_raw, byteorder="little", signed=True) / 100000000)

ser.close()