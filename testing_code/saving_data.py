import serial
import csv

arduino_port = "COM6"
baud = 9600
file_name = "sensor_data.csv"

ser = serial.Serial(arduino_port, baud)
print("Connected to arduino port: " + arduino_port)
file = open(file_name, "a")
print("Created file")

# display data to terminal
getData = ser.readline()
dataString = getData.decode('utf-8')
data = dataString[0:][:-2]
print(data)

readings = data.split(",")
print(readings)

samples = 20 #how many samples to collect
print_labels = False
line = 0 #start at 0 because our header is 0 (not real data)
sensor_data = [] #store data

# collect the samples
while line <= samples:
    getData=ser.readline()
    dataString = getData.decode('utf-8')
    data=dataString[0:][:-1]
    print(data)

    readings = data.split(",")
    print(readings)

    sensor_data.append(readings)
    print(sensor_data[-1][0])

    line = line+1

sensor_data.append(readings)
print(sensor_data)

# create CSV
with open(file_name, 'w', encoding='UTF8', newline='') as f:
    writer = csv.writer(f)
    writer.writerows(sensor_data)
    
print("Data collection complete!")
file.close()
