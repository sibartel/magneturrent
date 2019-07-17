import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import serial
import time
import struct

current_milli_time = lambda: int(round(time.time() * 1000))

style.use('fivethirtyeight')

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

ser = serial.Serial('/dev/ttyACM1', 115200)
ts = []
current = []

ser.reset_input_buffer()

def animate(i):
    while(ser.inWaiting() >= 6):
        magic_byte = struct.unpack('B', ser.read(1))[0]
        if magic_byte != 0xbe:
            continue
        
        status = struct.unpack('B', ser.read(1))[0]
        if status == 0:
            plt.text(0, 0, 'Not calibrated!', bbox=dict(facecolor='red', alpha=0.5), horizontalalignment='center', verticalalignment='center', transform=ax1.transAxes)

        timestamp = struct.unpack('<I', ser.read(4))[0]
        current.append(struct.unpack('<f', ser.read(4))[0])
        ts.append(timestamp)
        if len(ts) > 1000:
            ts.pop(0)
            current.pop(0)
    ax1.clear()
    plt.ylim([-1, 5])
    ax1.plot(ts, current)

ani = animation.FuncAnimation(fig, animate, interval=1)
plt.show()

ser.close()