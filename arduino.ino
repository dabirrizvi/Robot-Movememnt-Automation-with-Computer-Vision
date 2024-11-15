// Libraries for WiFi and Serial Communication
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Variables
int incomingByte = 0;  // Variable to store incoming serial data
int speed = 500;       // Motor speed (0 to 1023)

// Motor Pins
int enA = 16; // Enable pin for motor A (connected to pin D0)
int in1 = 5;  // Input 1 for motor A (connected to pin D1)
int in2 = 4;  // Input 2 for motor A (connected to pin D2)
int enB = 14; // Enable pin for motor B (connected to pin D5)
int in3 = 0;  // Input 1 for motor B (connected to pin D3)
int in4 = 2;  // Input 2 for motor B (connected to pin D4)

// WiFi Information
const char* ssid = "DABiR";        // WiFi network name
const char* password = "xxxxxxxxx"; // WiFi network password

WiFiUDP Udp;                         // UDP instance for network communication
unsigned int localUdpPort = 1998;    // Local port for incoming UDP packets
char incomingPacket[255];            // Buffer for storing incoming UDP data

void setup() {
    // Setup baud rate for serial communication
    Serial.begin(115200);
    delay(10);
    Serial.println("Motor test!");

    // WiFi Connection Setup
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);       // Start WiFi connection with the provided credentials
    while (WiFi.status() != WL_CONNECTED) { // Wait for connection
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());   // Display IP address assigned by WiFi network

    // UDP Configuration
    Serial.println("Starting UDP");
    Udp.begin(localUdpPort);          // Start UDP on the specified port

    // Configure motor control pins as outputs
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
}

// Motor Movement Functions
void right() {
    digitalWrite(in1, LOW);           // Set motor A to neutral
    digitalWrite(in2, LOW);
    analogWrite(enA, speed);          // Set motor A speed

    digitalWrite(in3, LOW);           // Set motor B to rotate right
    digitalWrite(in4, HIGH);
    analogWrite(enB, speed);          // Set motor B speed
}

void left() {
    digitalWrite(in1, LOW);           // Set motor A to rotate left
    digitalWrite(in2, HIGH);
    analogWrite(enA, speed);          // Set motor A speed

    digitalWrite(in3, LOW);           // Set motor B to neutral
    digitalWrite(in4, LOW);
    analogWrite(enB, speed);          // Set motor B speed
}

void backward() {
    digitalWrite(in1, HIGH);          // Set motor A to move backward
    digitalWrite(in2, LOW);
    analogWrite(enA, speed);          // Set motor A speed

    digitalWrite(in3, HIGH);          // Set motor B to move backward
    digitalWrite(in4, LOW);
    analogWrite(enB, speed);          // Set motor B speed
}

void forward() {
    digitalWrite(in1, LOW);           // Set motor A to move forward
    digitalWrite(in2, HIGH);
    analogWrite(enA, speed);          // Set motor A speed

    digitalWrite(in3, LOW);           // Set motor B to move forward
    digitalWrite(in4, HIGH);
    analogWrite(enB, speed);          // Set motor B speed
}

void stop() {
    // Stop both motors by setting all inputs to LOW
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

/* Packet Listening Function
   void ListenPacketRoutine(){
      // Listens for incoming UDP packets
      int packetSize = Udp.parsePacket();
      if (packetSize) {
          int len = Udp.read(incomingPacket, 255); // Read incoming packet data
          Serial.printf("UDP packet contents: %s\n", incomingPacket); // Display packet content

          // Check the first character in packet and call respective movement function
          if (incomingPacket[0] == 'b') {
              forward();
          } else if (incomingPacket[0] == 'f') {
              backward();
          } else if (incomingPacket[0] == 'l') {
              left();
          } else if (incomingPacket[0] == 'r') {
              right();
          } else if (incomingPacket[0] == 's') {
              stop();
          }
      }
  }
*/

/* Serial Listening Function
   void ListenKeyboardRoutine(){
      // Listens for incoming serial data
      if (Serial.available() > 0) {
          incomingByte = Serial.read(); // Read the incoming byte
      }
   
      switch(incomingByte) {
          case 's':
              stop();                   // Stop robot
              Serial.println("Stop\n");
              incomingByte='*';
              break;

          case 'f':
              forward();                // Move forward
              Serial.println("Forward\n");
              incomingByte='*';
              break;

          case 'b':  
              backward();               // Move backward
              Serial.println("Backward\n");
              incomingByte='*';
              break;

          case 'r':
              right();                  // Rotate right
              Serial.println("Rotate Right\n");
              incomingByte='*';
              break;

          case 'l':
              left();                   // Rotate left
              Serial.println("Rotate Left\n");
              incomingByte='*';
              break;
      }
  }
*/

void loop() {
    ListenPacketRoutine();    // Listen for UDP packets
    ListenKeyboardRoutine();   // Listen for serial keyboard commands
}