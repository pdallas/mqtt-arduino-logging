# MQTT Protocol Example
# Subscribing to a topic while saving in a microSD card everything that is published in that topic.
# Panagiotis-Ioannis Dallas, Undergraduate student in Digital Systems, University of Piraeus.

import paho.mqtt.client as mqtt
import psutil
import time
import os

cpu = 0
j = True
disk = psutil.disk_partitions()

print('Welcome, please select a hard drive. (Pick a choice from 1 to ' + str(len(disk)) + ')\n')
for i in range(len(disk)):  # printing drive options
    print("Drive " + str(i + 1) + ". " + disk[i][0])

while j:
    diskInput = int(input())  # saving user's drive choice

    if (diskInput - 1 < len(disk)) & (diskInput > 0):   # | if user's choice is true..
        diskSelected = str(disk[diskInput - 1][0])  # --->  | take the 'user input -1' list from the tuple
        j = False                                         # | and the first '0' value of the list (e.g. C:\\, D:\\ etc.)
    else:                                                 # | and save it into a string variable called diskSelected
        print("Wrong input, try again")

y = int(input("Number of System measurements?\n"))
os.system('cls')                                          # clear terminal


def on_connect(client, userdata, flags, rc):
    if rc == 0:                                           # rc == 0 no reconnection needed
        print('Connection established')
    else:
        print('Bad connection', rc)


broker = "192.168.1.6"                              # Setting up the broker's IP. It can be found in Ubuntu's terminal,
client = mqtt.Client("PythonClient")                # if you type ifconfig (sudo apt-get install net-tools)
client.on_connect = on_connect           # calling the on_connect function which is stated above^
print('Connecting to broker', broker)
client.connect(broker, port=1883)       # connecting the client with the broker,mosquitto's default broker port is 1883.
client.loop_start()  # connection stability

for x in range(y):
    # diskSelected is a string value like C:\\ D:\\ etc. Thats the input that psutil.disk_usage takes.
    diskUsage = psutil.disk_usage(diskSelected)
    # getting CPU utilization values, interval needs to be above 0.1 for accurate measurements.
    cpu = psutil.cpu_percent(interval=.5,percpu=False)
    # calculating time after epoch
    tim = time.asctime()

    if (client.publish("PythonToArduino", "Your " + str(disk[diskInput - 1][0]) + " Drive Usage is " + str(
            diskUsage.percent) + "%, CPU Utilization is " + str(cpu) + "%, " + str(tim) + ", Record ID: " + str(
            x + 1))):
        print('Sending message .. ', x + 1)
        # Publish to Topic Logs if the message was succesfully published from python client to arduino
        client.publish("logs", "<<PythonClient>> Computer's measurement record " +str(x+1) + " STATUS: Sent ")
    else:
        # else let Topic Logs know that there was a problem sending the message, by publishing an "Error" message.
        client.publish("logs","<<PythonClient>> STATUS: Error. ")

client.loop_stop()   # stop the client
client.disconnect()  # drop the connection with the broker
os.system('cls')     # clear the terminal screen
print('Disconnecting from broker ..')
print('Terminating..')
time.sleep(5)        # leaving some time to read the messages in the terminal
