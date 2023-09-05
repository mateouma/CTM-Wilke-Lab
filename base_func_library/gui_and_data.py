import tkinter as tk
import serial
from serial.tools import list_ports
import csv
import time
from datetime import datetime


# Serial connection
# Declare serial port variables
ports = serial.tools.list_ports.comports()

# Connect to Arduino port
ser = serial.Serial()

# Generate list of available ports
portsList = []
for onePort in ports:
    portsList.append(str(onePort))
    print(str(onePort))

# User-inputted port (ADD TO GUI)    
val = 3
for _, j in enumerate(portsList):
    if j.startswith("COM" + str(val)):
        portVar = "COM" + str(val)
        print(portVar)

# Windows Port Search:
# portVar = "COM" + str(input("Port: COM"))

# MacOS Port Search:
# portVar = "/dev/tty.usbmodem21301"
portVar = "/dev/cu.usbmodem1301"

ser.baudrate = 9600
ser.port = portVar
ser.open()

# =======================
# Functions
# =======================

def data_collection_ready():
    # Display data to terminal
    todayDateandTime = datetime.now()
    todayDateandTime = todayDateandTime.strftime("%m_%d_%Y-%H_%M_%S") 
    file_name = entry_file_name.get() + "_" + todayDateandTime + ".csv"
    if len(entry_file_name.get()) == 0:
        file_name = "test-" + todayDateandTime + ".csv" # Sets the file name to a default value incase it is not specified.    
    file = open(file_name, 'a')
    print("Created file with name", file_name)
    
    getData = ser.readline()
    print(getData)
    try:
        dataString = getData.decode('utf-8')
        data = dataString[0:][:-2]
    except:
        print("ERROR")
        window.destroy()
        return
    
    print(data)
    
    readings = data.split(",")
    print(readings)
    
    samples = int(entry_Samples.get())  # How many samples to collect (Set by the GUI)
    sensor_data = []  # Store data
    
    # trialsCompleted = 0
    
    # Collect the samples        
    # while trialsCompleted < samples:
    #     time.sleep(0.1)  # Add a small delay to ensure complete data collection
    #     getData = ser.readline()
    #     dataString = getData.decode('utf-8')
    #     data = dataString.strip()
    #     readings = data.split(",")
    #     sensor_data.append(readings)
    #     sensor_data_raw_text = sensor_data[-1][0]
    #     print(sensor_data_raw_text)
        
    #     if "Comp" in sensor_data_raw_text:
    #         trialsCompleted = int(sensor_data_raw_text[-1])

    while True:
        time.sleep(0.1)  # Add a small delay to ensure complete data collection
        getData = ser.readline()
        dataString = getData.decode('utf-8')
        data = dataString.strip()
        readings = data.split(",")
        sensor_data.append(readings)
        print(sensor_data[-1][0])

        currentTrial = data.split(":")
        # Add to CSV
        if currentTrial[0] == "Trial Completed":
            with open(file_name, 'a', encoding='UTF8', newline='') as f:
                writer = csv.writer(f)
                writer.writerows(sensor_data)
                sensor_data.clear()
            file.close()

        if currentTrial[0] == "Trial Completed" and int(currentTrial[1]) == samples:
            break

    print("Data collection complete!")
    file.close()
    ser.close()
    window.destroy()

def sendParams():    
    hr_pump_time = entry_time1.get()
    lr_pump_time = entry_time2.get()
    l_bar_height = entry_height1.get()
    r_bar_height = entry_height2.get()
    prob = entry_prob.get()
    hr_side = option_var.get()
    ITI = entry_ITI.get()
    delayTime = entry_delayTime.get()
    protocol_param_string = f'{hr_pump_time},{lr_pump_time},{l_bar_height},{r_bar_height},{prob},{hr_side},{ITI},{delayTime}'
    
    print(protocol_param_string)
    # Write protocol variables to Serial in form of a string
    ser.write(protocol_param_string.encode('utf-8'))
    time.sleep(2)  # Increase the delay before starting data collection
    data_collection_ready()
    return

# =======================
# GUI
# =======================

# Create the main window
window = tk.Tk()
window.title("Arduino Script Runner")

# Create the labels
tk.Label(window, text="HR Pump Time (ms):").grid(row=0, column=0, padx=10, pady=5)
tk.Label(window, text="LR Pump Time (ms):").grid(row=1, column=0, padx=10, pady=5)
tk.Label(window, text="Left Barrier Height (cm):").grid(row=2, column=0, padx=10, pady=5)
tk.Label(window, text="Right Barrier Height (cm):").grid(row=3, column=0, padx=10, pady=5)
tk.Label(window, text="Probability:").grid(row=4, column=0, padx=10, pady=5)
tk.Label(window, text="HR Side:").grid(row=5, column=0, padx=10, pady=5)
tk.Label(window, text="Inter-trial Interval:").grid(row=6, column=0, padx=10, pady=5)
tk.Label(window, text="Door Close Time Delay (ms):").grid(row=7, column=0, padx=10, pady=5)
tk.Label(window, text="Number of Samples:").grid(row=8, column=0, padx=10, pady=5)
tk.Label(window, text="File Name:").grid(row=9, column=0, padx=10, pady=5)
tk.Label(window, text=".csv").grid(row=9, column=2, padx=10, pady=5)
result_label = tk.Label(window, text="")
result_label.grid(row=10, column=0, padx=10, pady=5, columnspan=2)

# Create the entry fields
entry_time1 = tk.Entry(window)
entry_time1.grid(row=0, column=1, padx=10, pady=5)
entry_time2 = tk.Entry(window)
entry_time2.grid(row=1, column=1, padx=10, pady=5)
entry_height1 = tk.Entry(window)
entry_height1.grid(row=2, column=1, padx=10, pady=5)
entry_height2 = tk.Entry(window)
entry_height2.grid(row=3, column=1, padx=10, pady=5)
entry_prob = tk.Entry(window)
entry_prob.grid(row=4, column=1, padx=10, pady=5)
entry_ITI = tk.Entry(window)
entry_ITI.grid(row=6, column=1, padx=10, pady=5)
entry_delayTime = tk.Entry(window)
entry_delayTime.grid(row=7, column=1, padx=10, pady=5)
entry_Samples = tk.Entry(window)
entry_Samples.grid(row=8, column=1, padx=10, pady=5)
entry_file_name = tk.Entry(window)
entry_file_name.grid(row=9, column=1, padx=10, pady=5)

# Create the option buttons
option_var = tk.StringVar()
option_var.set(1) # Set the high side to Right

def set_option_1():
    option_var.set(1) # Set the high side to Right

def set_option_2():
    option_var.set(0) # Set the Hight side to Left.

option_button1 = tk.Radiobutton(window, text="Right", variable=option_var, value=1, command=set_option_1)
option_button1.grid(row=5, column=2, padx=10, pady=5)
option_button2 = tk.Radiobutton(window, text="Left", variable=option_var, value=0, command=set_option_2)
option_button2.grid(row=5, column=1, padx=10, pady=5)

# Create the button
button = tk.Button(window, text="Run Arduino Script", command=lambda: sendParams())
button.grid(row=10, column=1, padx=10, pady=5)
#button2 = tk.Button(window, text="Close Window", command=window.destroy())
#button2.grid(row=7, column=2, padx=10, pady=5)

# Start the GUI event loop
window.mainloop()
