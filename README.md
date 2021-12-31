# mqtt-arduino-logging
In this application, computer sensor data (CPU utilization, Drive Usage) are sent through an MQTT broker to an Arduino. If the Arduino is equipped with an Ethernet Shield, the incomming messages are being saved into a microSD. This setup can be easily used as a template example for the IoT world, as the Message Queuing Telemetry Transport (MQQT) protocol is designed as an extremely lightweight publish/subscribe messaging transport that is ideal for connecting remote devices with a small code footprint and minimal network bandwidth. MQTT today is used in a wide variety of industries.

A simple diagram is provided below:



![Untitled2](https://user-images.githubusercontent.com/64161512/147828958-746ae6ae-d7aa-439e-86ff-4cb9873e4b3f.png)

### STAGE 1 – MQTT BROKER SETUP in VM
Open the terminal in your Virtual Machine and use the following commands:

• sudo apt-get update

• sudo apt-get install mosquitto

• sudo apt-get install mosquitto-clients

After we install mosquitto, the broker is up and running. It starts every time automatically at startup. 

To subscribe to a topic, run:

• mosquitto_sub -d -u username -t test

In another terminal window, publish a test message to the topic "test" by running: 

• mosquitto_pub -d -u username -t test -m "Mosquitto Broker is Working"

### STAGE 2 – PC SETUP

In your computer, before you run the python script, configure your broker's IP and the topic that you would like to publish your computer's cpu/drive utilization data.

**Note:** If the host of the broker (probably a VM) is running on the same computer, configure its network adapter setting from NAT to Bridged Adapter and find the IP by running:

• Ifconfig 

### STAGE 3 – ARDUINO SETUP

Components required for this part: 

• Arduino Uno Board

• USB type-B Cable

• Arduino Ethernet Shield W5100

• Ethernet Cable

• MicroSD card (formatted to FAT16 or FAT32)

• MicroSD to SD card adapter

After connecting the Arduino to our computer, open the Arduino IDE and import the provided script.

All we have to do now is to download a library that we will need to connect to the Broker. By going to Tools / Manage Libraries a search bar appears. There we look for the PubSubClient library by Nick O’Leary, which we install. Like Stage 2, before compiling the script, configure broker's IP address and the sub topic.

Everything is now ready!

### Testing

First, run the python script and select drive and the number of measurements to send. 

![res3](https://user-images.githubusercontent.com/64161512/147830544-4ada4361-959b-47bb-94d2-e80b9ebc452d.png)


In the Virtual Machine terminal windows, we get: 

![res1](https://user-images.githubusercontent.com/64161512/147830592-36c5292a-da08-4860-acf9-600d1ee14cfd.png)

And in the microSD of the Arduino, we get:

![uni](https://user-images.githubusercontent.com/64161512/147831015-5298411d-5716-4d7a-afe9-747f4cdb8b95.jpg)


As we can see, everything runs as expected.

### Features

An interesting idea would be to add some sensors to the Arduino, like a thermometer, humidity sensor etc, publish their records through the MQTT and retrieve them in a remote time series database like InfluxDB. Then, the recorded data can be easily translated into time based charts with Grafana.



