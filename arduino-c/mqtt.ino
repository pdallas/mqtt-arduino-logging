//MQTT Protocol Example 
//Subscribing to a topic while saving in a microSD card everything that is published in that topic.
//Panagiotis-Ioannis Dallas, Undergraduate student in Digital Systems, University of Piraeus.

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <SD.h>

File myFile;                                            // Creating File object
void subscribeReceive(char* topic, byte* payload, unsigned int length);
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };    // Default mac address, must be unique
IPAddress ip(192, 168, 1, 2);                           // Arduino's IP, even if it's diffrent it wont effect this project
const char* server = "192.168.1.6";                     // Broker's IP, run ifconfig in Ubuntu's Terminal to find it
 
EthernetClient ethClient;                               // Creating Ethernet and MQTT related objects
PubSubClient mqttClient(ethClient);

void setup() {
  Serial.begin(9600);
  
  while (!Serial) {                                     // Wait for serial port to connect.                                 
    ;
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(4))                                    // Wait SD to connect, max 4 seconds
  {                                  
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
 
  Ethernet.begin(mac, ip);                              // Initiating ethernet connection
  delay(3000);                                          // Ethernet takes some time to boot, thats why we delay the process
 
  mqttClient.setServer(server, 1883);                   // Set the MQTT server to the server stated above, broker's default port is 1883 
  if (mqttClient.connect("Arduino"))                    // Attempt to connect to the server with the ID "Arduino" , if true conn.established
  {
    Serial.println("Connection has been established, well done");
    mqttClient.setCallback(subscribeReceive);           // Establish the subscribe event
  } 
  else 
  {
    Serial.println("Looks like the server connection failed...");
  } 
}                                                      // End of setup()

void loop() {
  mqttClient.loop();                                   // Keeping the connection up 
  mqttClient.subscribe("PythonToArduino");             // Subscribe to topic "PythonToArduino"
}                                                      // End of loop()

void subscribeReceive(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  myFile = SD.open("cpu.txt", FILE_WRITE);            // SD.open can create cpu.txt if it does not exist or open the file
  
  for(int i = 0; i < length; i ++)
  {
    Serial.print(char(payload[i]));
    if (myFile) 
    {
        myFile.print(char(payload[i]));               // If myFile is true that means its open, print every char of the message recieved
    }
    else
    {
        Serial.println("Error opening file");         // If myFile is closed break (It happens if we try to create a file with over 7-8 letters.
        break;
    }
   }
  myFile.println("");                                 // Create a new line (for the next message)
  myFile.close();                                     // Close the file
  mqttClient.publish("logs", "<<ArduinoClient>> Cpu measurement recieved and saved in MicroSD"); // Publish to topic Logs that the message arrived
  Serial.println(" Record Saved in MicroSD.");
  
}
