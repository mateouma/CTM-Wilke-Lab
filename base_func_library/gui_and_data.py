import tkinter as tk
import os
import serial
from serial.tools import list_ports
import csv
import time
from datetime import datetime
from sys import platform
import subprocess
if platform == "win32":
    import pywinauto
import random

# # This doesn't work ... need to fix.

# import signal
# import sys

# # Signal handler
# def signal_handler(sig, frame):
#     signal.signal(signal.SIGINT, original_sigint)

#     try:
#         if input("\nReally quit? (y/n)> ").lower().startswith('y'):
#             sys.exit(1)
#     except KeyboardInterrupt:
#         print("Ok ok, quitting")
#         sys.exit(1)

#     # val = str(input('\n\nAre you sure that you want to close out of the trial and end data collection? [y/n]\n\n'))
#     # if val == "y" or val == "yes":
#     #     print("Exiting...\n\n")
#     #     sys.exit(0)
#     # if val == "n" or val == "no":
#     #     print("Resuming...\n\n")

#     signal.signal(signal.SIGINT, signal_handler)


# original_sigint = signal.getsignal(signal.SIGINT)
# signal.signal(signal.SIGINT, signal_handler)

onSensor = ""
offSensor = ""
onTimeDelay = 0
offTimeDelay = 0
laserSide = "Both HR and LR Sides" # Both sides turn on
laserProb = 0
laserPulseType = 0 #0 is constant and 1 is pulse
laserMode = "" #If it is manual or sensor event

file_name = ""
total_samples = 0


# Serial connection
# Declare serial port variables
ports = serial.tools.list_ports.comports()

# Connect to Arduino port
ser = serial.Serial()

# Generate list of available ports
portsList = []
for onePort in ports:
    portsList.append(str(onePort))
    # print(str(onePort))

# User-inputted port (ADD TO GUI)    
val = 3
for _, j in enumerate(portsList):
    if j.startswith("COM" + str(val)):
        portVar = "COM" + str(val)
        # print(portVar)

# if platform == "win32":
#     # Windows Port Search:
#     portVar = "COM" + str(input("Port: COM"))
# elif platform == "darwin":
#     # MacOS Port Search:
#     portVar = "/dev/cu.usbmodem" + str(input("Port: /dev/cu.usbmodem"))
# else:
#     print("Unsupported operating system")
#     quit()

# ser.baudrate = 9600
# ser.port = portVar
# ser.open()

# =======================
# Functions
# =======================
def begin_recording():
    # Starting camera app:
    if platform == "win32":
        subprocess.run('start microsoft.windows.camera:', shell=True)
        time.sleep(2)
        desktop = pywinauto.Desktop(backend="uia")
        cam = desktop['Camera']

        successfulRecording = False
        for i in range(5):
            # make sure in video mode
            if cam.child_window(title="Switch to video mode", auto_id="CaptureButton_1", control_type="Button").exists():
                cam.child_window(title="Switch to video mode", auto_id="CaptureButton_1", control_type="Button").click()
                time.sleep(1)
            # start then stop video
            if cam.child_window(title="Take video", auto_id="CaptureButton_1", control_type="Button").exists():
                cam.child_window(title="Take video", auto_id="CaptureButton_1", control_type="Button").click()
                successfulRecording = True
                print("Started Recording!")
            else:
                time.sleep(5)
        
        if not successfulRecording:
            print("\n\n\nFAILED TO START RECORDING, PLEASE RECORD MANUALLY!!!\n\n\n")

def end_recording():
    # Stop recording and close out the camera.
    if platform == "win32":
        desktop = pywinauto.Desktop(backend="uia")
        cam = desktop['Camera']
        if cam.child_window(title="Stop taking video", auto_id="CaptureButton_1", control_type="Button").exists():
            cam.child_window(title="Stop taking video", auto_id="CaptureButton_1", control_type="Button").click()
            time.sleep(2)
            subprocess.run('Taskkill /IM WindowsCamera.exe /F', shell=True)  # close camera app
            print('Recorded successfully!')
        else:
            print("Issue with recording, likely turned off manually. Not guaranteed to be saved.")
            time.sleep(2)
            subprocess.run('Taskkill /IM WindowsCamera.exe /F', shell=True)  # close camera app


def data_collection_ready():
    global file_name, total_samples
    # Display data to terminal
    todayDateandTime = datetime.now()
    todayDateandTime = todayDateandTime.strftime("%m_%d_%Y-%H_%M_%S") 
    if len(file_name) == 0:
        file_name = "test-" + todayDateandTime + ".csv" # Sets the file name to a default value incase it is not specified.    
    else:
        file_name = file_name + "_" + todayDateandTime + ".csv" 
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
    
    samples = total_samples  # How many samples to collect (Set by the GUI)
    sensor_data = []  # Store data
    
    lr_choice = 0
    hr_choice = 0


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

        if currentTrial[0] == "HR Side Was Chosen":
            hr_choice = hr_choice + 1
        elif currentTrial[0] == "LR Side Was Chosen":
            lr_choice = lr_choice + 1

        if currentTrial[0] == "Trial Completed" and int(currentTrial[1]) == samples:
            break

    ser.write("Data Complete".encode('utf-8'))
    print("\n\n\nData collection complete!")
    print("This mouse picked the HR side in", "{:.2%}".format(hr_choice/samples), "of the trials.\n\n\n")
    time.sleep(2)
    end_recording()

    file.close()
    ser.close()
    window.destroy()
    os._exit(00)

def laser_OffSetUp(window):
    def submit_laser_off_params(window):
        global offSensor, offTimeDelay
        offSensor = sensor_off_selected.get()
        offTimeDelay = laser_off_time_delay_selected.get()
        print(onSensor)
        print(onTimeDelay)

        window.destroy()
        return

    window.destroy()

    window = tk.Tk()
    window.title("Automate T-Maze Laser Set-Up")
    tk.Label(window, text=f'Setting Up When the Laser Turns Off').grid(row=0, column=1, padx=10, pady=5)

    # Labels:
    tk.Label(window, text="Which Sensor Should Turn the Laser Off?").grid(row=1, column=0, padx=10, pady=5)
    tk.Label(window, text="Time Delay Between Sensor Event and Laser Off (seconds):").grid(row=2, column=0, padx=10, pady=5)

    if onSensor == "Start":
        sensor_off_Options = ["Midstem", "Endstem", "Post Vertex", "Pre-Barrier", "Start Box"]
    elif onSensor == "Midstem":
        sensor_off_Options = ["Endstem", "Post Vertex", "Pre-Barrier", "Start Box"]
    elif onSensor == "Endstem":
        sensor_off_Options = ["Post Vertex", "Pre-Barrier", "Start Box"]
    elif onSensor == "Post Vertex":
        sensor_off_Options = ["Pre-Barrier", "Start Box"]
    elif onSensor == "Pre-Barrier":
        sensor_off_Options = ["Start Box"]

    sensor_off_selected = tk.StringVar(window) 
    sensor_off_selected.set(sensor_off_Options[0]) 
    sensor_off_dropdown_menu = tk.OptionMenu(window, sensor_off_selected, *sensor_off_Options) 
    sensor_off_dropdown_menu.grid(row=1, column=1, padx=10, pady=5)

    laser_off_time_delay_selected = tk.Entry(window)
    laser_off_time_delay_selected.grid(row=2, column=1, padx=10, pady=5)

    button = tk.Button(window, text="Submit", command=lambda: submit_laser_off_params(window))
    button.grid(row=3, column=1, padx=10, pady=5)

    window.mainloop()

def laser_OnSetUp(window):
    def submit_laser_on_params(window):
        global onSensor, onTimeDelay, laserSide
        onSensor = sensor_on_selected.get()
        onTimeDelay = laser_on_time_delay_selected.get()
        laserSide = laser_trial_type_selected.get()
        print(onSensor)
        print(onTimeDelay)
        print(laserSide)

        laser_OffSetUp(window)

    window.destroy()

    window = tk.Tk()
    window.title("Automate T-Maze Laser Set-Up")
    tk.Label(window, text=f'Setting Up When the Laser Turns On').grid(row=0, column=1, padx=10, pady=5)

    # Labels:
    tk.Label(window, text="Which Sensor Should Turn the Laser On?").grid(row=1, column=0, padx=10, pady=5)
    tk.Label(window, text="Time Delay Between Sensor Event and Laser On (seconds):").grid(row=2, column=0, padx=10, pady=5)
    tk.Label(window, text="Which Type of Trial Should Turn the Laser On?").grid(row=3, column=0, padx=10, pady=5)

    sensor_on_Options = ["Start", "Midstem", "Endstem", "Post Vertex", "Pre-Barrier"]
    sensor_on_selected = tk.StringVar(window) 
    sensor_on_selected.set("Start") 
    sensor_on_dropdown_menu = tk.OptionMenu(window, sensor_on_selected, *sensor_on_Options) 
    sensor_on_dropdown_menu.grid(row=1, column=1, padx=10, pady=5)

    laser_on_time_delay_selected = tk.Entry(window)
    laser_on_time_delay_selected.grid(row=2, column=1, padx=10, pady=5)
    
    laser_trial_type_Options = ["Both HR and LR Sides", "HR Side", "LR Side"]
    laser_trial_type_selected = tk.StringVar(window) 
    laser_trial_type_selected.set("Both HR and LR Sides") 
    laser_trial_type_dropdown_menu = tk.OptionMenu(window, laser_trial_type_selected, *laser_trial_type_Options) 
    laser_trial_type_dropdown_menu.grid(row=3, column=1, padx=10, pady=5)

    button = tk.Button(window, text="Submit", command=lambda: submit_laser_on_params(window))
    button.grid(row=4, column=1, padx=10, pady=5)

    window.mainloop()


def laserSetUp(window):
    def input_laser_on_params(window):
        global laserProb, laserPulseType, laserMode
        laserMode = laser_mode_selected.get()
        laserPulseType = laserPulse_selected.get()
        laserProb = float(laserProb_selected.get())
        print(laserMode)
        print(laserPulseType)
        print(laserProb)
        print("made it")

        if(laserMode == "Manual Mode"):
            window.destroy()
            return
        elif(laserMode == "Turn on by a sensor event"):
            laser_OnSetUp(window)
        else:
            print("Error with Laser Mode")
            quit()
    
    window.destroy()

    window = tk.Tk()
    window.title("Automate T-Maze Laser Set-Up")
    tk.Label(window, text=f'Setting Up When the Laser Turns On').grid(row=0, column=1, padx=10, pady=5)

    # Labels:
    tk.Label(window, text="When should the laser turn on?").grid(row=1, column=0, padx=10, pady=5)
    tk.Label(window, text="What pulse type should the laser have?").grid(row=2, column=0, padx=10, pady=5)
    tk.Label(window, text="What is the percentage of trials that should have the laser? (%)").grid(row=3, column=0, padx=10, pady=5)

    laser_Mode_Options = ["Turn on by a sensor event", "Manual Mode"]
    laser_mode_selected = tk.StringVar(window) 
    laser_mode_selected.set("Turn on by a sensor event") 
    laser_mode_dropdown_menu = tk.OptionMenu(window, laser_mode_selected, *laser_Mode_Options) 
    laser_mode_dropdown_menu.grid(row=1, column=1, padx=10, pady=5)

    # Create the option buttons
    laserPulse_selected = tk.StringVar()
    laserPulse_selected.set(0) # Set the high side to Const
    def set_pulse_1():
        laserPulse_selected.set(0) # Set the high side to Const

    def set_pulse_2():
        laserPulse_selected.set(1) # Set the Hight side to Pulse.\
    laserPulse_button1 = tk.Radiobutton(window, text="Constant", variable=laserPulse_selected, value=0, command=set_pulse_1)
    laserPulse_button1.grid(row=2, column=1, padx=10, pady=5)
    laserPulse_button2 = tk.Radiobutton(window, text="Pulse", variable=laserPulse_selected, value=1, command=set_pulse_2)
    laserPulse_button2.grid(row=2, column=2, padx=10, pady=5)
    
    laserProb_selected = tk.Entry(window)
    laserProb_selected.grid(row=3, column=1, padx=10, pady=5)

    button = tk.Button(window, text="Submit", command=lambda: input_laser_on_params(window))
    button.grid(row=4, column=1, padx=10, pady=5)

    window.mainloop()

def sendParams(window):    
    global file_name, total_samples
    hr_pump_time = entry_time1.get()
    lr_pump_time = entry_time2.get()
    l_bar_height = entry_height1.get()
    r_bar_height = entry_height2.get()
    prob_HR = entry_prob_HR.get()
    prob_LR = entry_prob_LR.get()
    hr_side = option_var.get()
    ITI = entry_ITI.get()
    delayTime = entry_delayTime.get()
    total_samples = entry_Samples.get()
    file_name = entry_file_name.get()
    laser_selected = entry_laser_selected.get()

    if(len(hr_pump_time) == 0 or len(lr_pump_time) == 0 or len(l_bar_height) == 0 or len(r_bar_height) == 0 
       or len(prob_HR) == 0 or len(prob_LR) == 0 or len(hr_side) == 0 or len(ITI) == 0 
       or len(delayTime) == 0 or len(total_samples) == 0):
        print("Please input a value for all parameters")
        quit()
    
    total_samples = int(total_samples)
    #Take just the port value from the selected
    selected_port = port_selected.get().split(' ')[0]
    if selected_port == "Select": 
        print("Please Select a Port")
        quit()

    print(laser_selected)
    
    # Finish the string input:
    if laser_selected == "Yes":
        laserSetUp(window)

    #Need array of which trial will receive laser 
    # laser_on_array=[]
    print(laserProb)
    num_laser_on = int(laserProb * total_samples)
    num_laser_off = total_samples - num_laser_on 
    laser_on_array = [1] * num_laser_on + [0] * num_laser_off
    random.shuffle(laser_on_array)

    print(laser_on_array)


    
    #Laser function will update global values:
    protocol_param_string = f'{hr_pump_time},{lr_pump_time},{l_bar_height},{r_bar_height},{prob_HR},{prob_LR},{hr_side},{ITI},{delayTime},{laser_selected},{laserMode},{laserPulseType},{laserSide},{onSensor},{onTimeDelay},{offSensor},{offTimeDelay},{laser_on_array}'
    
    ser.baudrate = 9600
    ser.port = selected_port
    ser.open()
        
    print(protocol_param_string)
    # Write protocol variables to Serial in form of a string
    ser.write(protocol_param_string.encode('utf-8'))
    begin_recording()
    time.sleep(5)  # Increase the delay before starting data collection
    data_collection_ready()
    return

def sendTestParams():    
    protocol_param_string = "1000,500,0,0,1,0,1,100,100"
    
    print(protocol_param_string)
    # Write protocol variables to Serial in form of a string
    ser.write(protocol_param_string.encode('utf-8'))
    time.sleep(2)  # Increase the delay before starting data collection
    begin_recording()
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
tk.Label(window, text="HR Probability on HR Side:").grid(row=4, column=0, padx=10, pady=5)
tk.Label(window, text="HR Probability on LR Side:").grid(row=5, column=0, padx=10, pady=5)
tk.Label(window, text="HR Side:").grid(row=6, column=0, padx=10, pady=5)
tk.Label(window, text="Inter-trial Interval:").grid(row=7, column=0, padx=10, pady=5)
tk.Label(window, text="Door Close Time Delay (ms):").grid(row=8, column=0, padx=10, pady=5)
tk.Label(window, text="Number of Samples:").grid(row=9, column=0, padx=10, pady=5)
tk.Label(window, text="File Name:").grid(row=10, column=0, padx=10, pady=5)
tk.Label(window, text=".csv").grid(row=10, column=2, padx=10, pady=5)
tk.Label(window, text="Would you like to use the laser?").grid(row=11, column=0, padx=10, pady=5)
tk.Label(window, text="Select the connected port:").grid(row=12, column=0, padx=10, pady=5)
result_label = tk.Label(window, text="")
result_label.grid(row=13, column=0, padx=10, pady=5, columnspan=2)

# Create the entry fields
entry_time1 = tk.Entry(window)
entry_time1.grid(row=0, column=1, padx=10, pady=5)
entry_time2 = tk.Entry(window)
entry_time2.grid(row=1, column=1, padx=10, pady=5)
entry_height1 = tk.Entry(window)
entry_height1.grid(row=2, column=1, padx=10, pady=5)
entry_height2 = tk.Entry(window)
entry_height2.grid(row=3, column=1, padx=10, pady=5)
entry_prob_HR = tk.Entry(window)
entry_prob_HR.grid(row=4, column=1, padx=10, pady=5)
entry_prob_LR = tk.Entry(window)
entry_prob_LR.grid(row=5, column=1, padx=10, pady=5)
entry_ITI = tk.Entry(window)
entry_ITI.grid(row=7, column=1, padx=10, pady=5)
entry_delayTime = tk.Entry(window)
entry_delayTime.grid(row=8, column=1, padx=10, pady=5)
entry_Samples = tk.Entry(window)
entry_Samples.grid(row=9, column=1, padx=10, pady=5)
entry_file_name = tk.Entry(window)
entry_file_name.grid(row=10, column=1, padx=10, pady=5)

entry_laser_selected = tk.StringVar(window) 
entry_laser_selected.set("No") 
optionList = ["No", "Yes"]
dropdown_menu = tk.OptionMenu(window, entry_laser_selected, *optionList) 
dropdown_menu.grid(row=11, column=1, padx=10, pady=5)

port_selected = tk.StringVar(window) 
port_selected.set("Select an Option") 
dropdown_menu = tk.OptionMenu(window, port_selected, *portsList) 
dropdown_menu.grid(row=12, column=1, padx=10, pady=5)

# Create the option buttons for HR and LR
option_var = tk.StringVar()
option_var.set(1) # Set the high side to Right

def set_option_1():
    option_var.set(1) # Set the high side to Right

def set_option_2():
    option_var.set(0) # Set the Hight side to Left.

option_button1 = tk.Radiobutton(window, text="Right", variable=option_var, value=1, command=set_option_1)
option_button1.grid(row=6, column=2, padx=10, pady=5)
option_button2 = tk.Radiobutton(window, text="Left", variable=option_var, value=0, command=set_option_2)
option_button2.grid(row=6, column=1, padx=10, pady=5)

# Create the button
button = tk.Button(window, text="Run Arduino Script", command=lambda: sendParams(window))
button.grid(row=13, column=1, padx=10, pady=5)
button2 = tk.Button(window, text="Run Testing Script", command=lambda: sendTestParams())
button2.grid(row=14, column=1, padx=10, pady=5)

#button2 = tk.Button(window, text="Close Window", command=window.destroy())
#button2.grid(row=7, column=2, padx=10, pady=5)

# Start the GUI event loop
window.mainloop()