import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import serial
import time
import struct
import sys

if len(sys.argv) != 2:
    print("Usage: ", sys.argv[0], " <serial device>")
    sys.exit()

style.use('fivethirtyeight')

fig = plt.figure(num="Current Scope")
ax1 = fig.add_subplot(1,1,1)

ser = serial.Serial(sys.argv[1], 115200)
ts = []
current = []

ser.reset_input_buffer()

def animate(i):
    global ts
    global current
    ax1.clear()
    while(ser.inWaiting() >= 6):
        magic_byte = struct.unpack('B', ser.read(1))[0]
        if magic_byte != 0xbe:
            continue
        
        status = struct.unpack('B', ser.read(1))[0]
        timestamp = struct.unpack('<I', ser.read(4))[0]
        current.append(struct.unpack('<f', ser.read(4))[0])

        if status != 2:
            if status == 0:
                plt.text(0.5, 0.5, 'Not calibrated!', bbox=dict(facecolor='red', alpha=0.5), horizontalalignment='center', verticalalignment='center', transform=ax1.transAxes)
            else:
                plt.text(0.5, 0.5, 'Calibration ongoing!', bbox=dict(facecolor='orange', alpha=0.5), horizontalalignment='center', verticalalignment='center', transform=ax1.transAxes)
            ts = []
            current = []
            continue

        ts.append(timestamp)
        if len(ts) > 1000:
            ts.pop(0)
            current.pop(0)
    plt.ylim([-1, 6])
    ax1.plot(ts, current)

ani = animation.FuncAnimation(fig, animate, interval=1)
plt.show()

ser.close()