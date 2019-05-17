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
xs = []
ys = []

ser.reset_input_buffer()

def animate(i):
    data_raw = ser.read(4)
    data = int.from_bytes(data_raw, byteorder="little", signed=True)
    xs.append(current_milli_time())
    ys.append(data)
    if len(xs) > 100:
        xs.pop(0)
        ys.pop(0)
    ax1.clear()
    ax1.plot(xs, ys)

ani = animation.FuncAnimation(fig, animate, interval=100)
plt.show()

ser.close()