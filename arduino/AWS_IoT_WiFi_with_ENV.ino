/*
  AWS IoT WiFi with MKR WiFi 1010 and MKR ENV SHIELD

  This sketch securely connects to an AWS IoT using MQTT over WiFi.
  It uses a private key stored in the ATECC508A and a public
  certificate for SSL/TLS authetication.
	
  Once authenticated with the AWS broker, it publishes 7 JSON messages containing output 
  from each of the sensors on the MKR ENV SHIELD to arduino/environmental every 5 minutes.

  The fields included in the JSON messages are as follows:
  - type (measurement such as temperature, humidity etc.)
  - timestamp (epoch time, seconds since Jan 1 1970)
  - value (the value of the measurement)

  The circuit:
  - Arduino MKR WiFi 1010
  - Arduino MKR ENV SHIELD

  This code is adapted from the AWS IoT Wifi template for the MKR WiFi 1010 and MKR WiFi 1000 boards.
*/

#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h> // change to #include <WiFi101.h> for MKR1000
#include <Arduino_MKRENV.h>

#include "arduino_secrets.h"

/////// Enter your sensitive data in arduino_secrets.h
const char ssid[]        = SECRET_SSID;
const char pass[]        = SECRET_PASS;
const char broker[]      = SECRET_BROKER;
const char* certificate  = SECRET_CERTIFICATE;

WiFiClient    wifiClient;            // Used for the TCP socket connection
BearSSLClient sslClient(wifiClient); // Used for SSL/TLS connection, integrates with ECC508
MqttClient    mqttClient(sslClient);

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    }

  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }

  // Set a callback to get the current time
  // used to validate the servers certificate
  ArduinoBearSSL.onGetTime(getTime);

  // Set the ECCX08 slot to use for the private key
  // and the accompanying public certificate for it
  sslClient.setEccSlot(0, certificate);

  // Optional, set the client id used for MQTT,
  // each device that is connected to the broker
  // must have a unique client id. The MQTTClient will generate
  // a client id for you based on the millis() value if not set
  //
  // mqttClient.setId("clientId");

  // Set the message callback, this function is
  // called when the MQTTClient receives a message
  mqttClient.onMessage(onMessageReceived);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect
    connectMQTT();
  }

  // poll for new MQTT messages and send keep alives
  mqttClient.poll();

  // publish a message roughly every 5 seconds.
  if (millis() - lastMillis > 300000) {
    lastMillis = millis();

    publishMessage();
  }
}

unsigned long getTime() {
  // get the current time from the WiFi module  
  return WiFi.getTime();
}

void connectWiFi() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(ssid);
  Serial.print(" ");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the network");
  Serial.println();
}

void connectMQTT() {
  Serial.print("Attempting to MQTT broker: ");
  Serial.print(broker);
  Serial.println(" ");

  while (!mqttClient.connect(broker, 8883)) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the MQTT broker");
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe("arduino/incoming");
}

void publishMessage() {
  Serial.println("Publishing message");

  //Capture values from MKR ENV SHIELD board
  float temperature = ENV.readTemperature();
  float humidity    = ENV.readHumidity();
  float pressure    = ENV.readPressure();
  float illuminance = ENV.readIlluminance();
  float uva         = ENV.readUVA();
  float uvb         = ENV.readUVB();
  float uvIndex     = ENV.readUVIndex();

  //Serial.print(temperature);

  //Serial.print(illuminance);

  //Serial.print(humidity);
  
  // Send JSON messages, the Print interface can be used to set the message contents
  // Could probably be done more elegantly with a loop
  mqttClient.beginMessage("arduino/environmental");
  mqttClient.print("{");
  mqttClient.print("\"type\" : \"temperature\" ");
  mqttClient.print(", ");  
  mqttClient.print("\"timestamp\" : ");
  mqttClient.print(WiFi.getTime());
  mqttClient.print(", ");  
  mqttClient.print("\"value\" : ");
  mqttClient.print(temperature);
  mqttClient.print("}"); 
  mqttClient.endMessage();
  
  mqttClient.beginMessage("arduino/environmental");
  mqttClient.print("{");
  mqttClient.print("\"type\" : \"humidity\" ");
  mqttClient.print(", ");  
  mqttClient.print("\"timestamp\" : ");
  mqttClient.print(WiFi.getTime());
  mqttClient.print(", ");  
  mqttClient.print("\"value\" : ");
  mqttClient.print(humidity);
  mqttClient.print("}"); 
  mqttClient.endMessage();
  
  mqttClient.beginMessage("arduino/environmental");
  mqttClient.print("{");
  mqttClient.print("\"type\" : \"pressure\" ");
  mqttClient.print(", ");  
  mqttClient.print("\"timestamp\" : ");
  mqttClient.print(WiFi.getTime());
  mqttClient.print(", ");  
  mqttClient.print("\"value\" : ");
  mqttClient.print(pressure);
  mqttClient.print("}"); 
  mqttClient.endMessage();
  
  mqttClient.beginMessage("arduino/environmental");
  mqttClient.print("{");
  mqttClient.print("\"type\" : \"illuminance\" ");
  mqttClient.print(", ");  
  mqttClient.print("\"timestamp\" : ");
  mqttClient.print(WiFi.getTime());
  mqttClient.print(", ");  
  mqttClient.print("\"value\" : ");
  mqttClient.print(illuminance);
  mqttClient.print("}"); 
  mqttClient.endMessage();
  
  mqttClient.beginMessage("arduino/environmental");
  mqttClient.print("{");
  mqttClient.print("\"type\" : \"uva\" ");
  mqttClient.print(", ");  
  mqttClient.print("\"timestamp\" : ");
  mqttClient.print(WiFi.getTime());
  mqttClient.print(", ");  
  mqttClient.print("\"value\" : ");
  mqttClient.print(uva);
  mqttClient.print("}"); 
  mqttClient.endMessage();
  
  mqttClient.beginMessage("arduino/environmental");
    mqttClient.print("{");
  mqttClient.print("\"type\" : \"uvb\" ");
  mqttClient.print(", ");  
  mqttClient.print("\"timestamp\" : ");
  mqttClient.print(WiFi.getTime());
  mqttClient.print(", ");  
  mqttClient.print("\"value\" : ");
  mqttClient.print(uvb);
  mqttClient.print("}"); 
  mqttClient.endMessage();
  
  mqttClient.beginMessage("arduino/environmental");
  mqttClient.print("{");
  mqttClient.print("\"type\" : \"uvIndex\" ");
  mqttClient.print(", ");  
  mqttClient.print("\"timestamp\" : ");
  mqttClient.print(WiFi.getTime());
  mqttClient.print(", ");  
  mqttClient.print("\"value\" : ");
  mqttClient.print(uvIndex);
  mqttClient.print("}"); 
  mqttClient.endMessage(); 
}

void onMessageReceived(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
  }
  Serial.println();

  Serial.println();
}
