
import matplotlib.pyplot as plt
MAX_DATA_POINTS = 100
import serial
import time

ser = serial.Serial('/dev/tty.usbmodem2101', 9600, timeout=1)
time.sleep(2)

user_data = input("Enter Mode: ")
command = user_data
ser.write(command.encode()) 
if command == '3':
    print("Data Logging Mode Selected")
    plt.ion()  # interactive mode ON
    time_data = []
    temp_data = []
    buzzer_data = []
    start_time = time.time()
    fig, ax = plt.subplots()
    temp_line, = ax.plot([], [])
    buzzer_line, = ax.plot([], [])
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("Temperature (°F) and Buzzer State (0 [OFF], 1 [ON])")
    while True:
        buzzer_raw = ser.readline()
        temp_raw = ser.readline()
        if buzzer_raw and temp_raw:
            buzzer_str = buzzer_raw.decode().strip()
            temp_str = temp_raw.decode().strip()
            try:
                buzzer = int(buzzer_str)
                temperature = float(temp_str)
                current_time = time.time() - start_time
                time_data.append(current_time)
                temp_data.append(temperature)
                buzzer_data.append(buzzer)

                if len(time_data) > MAX_DATA_POINTS:
                    time_data.pop(0)
                    temp_data.pop(0)
                    buzzer_data.pop(0)
                temp_line.set_xdata(time_data)
                temp_line.set_ydata(temp_data)
                buzzer_line.set_xdata(time_data)
                buzzer_line.set_ydata(buzzer_data)
                ax.relim()
                ax.autoscale_view()
                plt.draw()
                plt.pause(0.1)
            except ValueError:
                print(f"Invalid data received: {buzzer_str}, {temp_str}")





