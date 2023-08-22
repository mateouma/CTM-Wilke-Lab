# -*- coding: utf-8 -*-
"""
Created on Tue Jun  6 12:05:30 2023

@author: mauma
"""

import tkinter as tk
import serial

def run_arduino_script(option):
    # Retrieve the values from the fields
    time1 = entry_time1.get()
    time2 = entry_time2.get()
    height1 = entry_height1.get()
    height2 = entry_height2.get()
    prob = entry_prob.get()

    # Validate the inputs
    if not all([time1, time2, height1, height2, prob]):
        result_label.config(text="Please fill in all the fields.")
        return

    # Open serial connection to Arduino
    try:
        ser = serial.Serial('COM4', 9600)  # Replace 'COM3' with the actual serial port of your Arduino
        result_label.config(text="Serial connection established.")
    except serial.SerialException:
        result_label.config(text="Error occurred while opening serial connection.")
        return

    # Send the parameters to Arduino
    data = f"{option},{time1},{time2},{height1},{height2},{prob},\n"
    ser.write(data.encode())
    print(data)

    # Close the serial connection
    ser.close()
    result_label.config(text="Arduino script executed successfully.")

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
result_label = tk.Label(window, text="")
result_label.grid(row=7, column=0, padx=10, pady=5, columnspan=2)

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

# Create the option buttons
option_var = tk.StringVar()
option_var.set("R")

def set_option_1():
    option_var.set("R")

def set_option_2():
    option_var.set("L")

option_button1 = tk.Radiobutton(window, text="Right", variable=option_var, value="R", command=set_option_1)
option_button1.grid(row=5, column=1, padx=10, pady=5)
option_button2 = tk.Radiobutton(window, text="Left", variable=option_var, value="L", command=set_option_2)
option_button2.grid(row=5, column=2, padx=10, pady=5)

# Create the button
button = tk.Button(window, text="Run Arduino Script", command=lambda: run_arduino_script(option_var.get()))
button.grid(row=7, column=1, padx=10, pady=5)

# Start the GUI event loop
window.mainloop()