import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import serial
import time

current_milli_time = lambda: int(round(time.time() * 1000))

style.use('fivethirtyeight')

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

ser = serial.Serial('/dev/ttyACM2', 115200)
ts = []
x = []
y = []
z = []

ser.reset_input_buffer()

def animate(i):
    x_raw = ser.read(4)
    y_raw = ser.read(4)
    z_raw = ser.read(4)
    x.append(int.from_bytes(x_raw, byteorder="little", signed=True) / 100000000)
    y.append(int.from_bytes(y_raw, byteorder="little", signed=True) / 100000000)
    z.append(int.from_bytes(z_raw, byteorder="little", signed=True) / 100000000)

    ts.append(current_milli_time())
    if len(ts) > 100:
        ts.pop(0)
        x.pop(0)
        y.pop(0)
        z.pop(0)
    ax1.clear()
    ax1.plot(ts, x)
    ax1.plot(ts, y)
    ax1.plot(ts, z)

ani = animation.FuncAnimation(fig, animate, interval=100)
plt.show()

ser.close()