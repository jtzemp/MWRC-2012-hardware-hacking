/*
 MWRC CI Server bot
 created by JT Zemp
 
 Licensed under MIT Open Source License
 
 based on Arduino example code from David A. Mellis created on 18 Dec 2009
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x8D, 0x5A };

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to 
EthernetClient client;

IPAddress server(192,168,2,1); // local machine
//char server[] = "mwrc-ci.heroku.com"; // remote machine
int port = 4567;

const int requestInterval = 10000;  // delay between requests 10 seconds
boolean requested;                  // whether you've made a request since connecting
long lastAttemptTime = 0;           // last time you connected to the server, in milliseconds
String currentLine = "";            // string to hold the text from server, line by line
String color = "";                  // string to hold the color as it's being received
String currentColor = "red";           // string to hold the finished color
boolean readingColor = false;       // if you're currently reading the color

// The pins for PWM color
#define RED   3
#define GREEN 5
#define BLUE  6


/* 
  The setup() function is one of the two required functions for Arduino.
  setup() runs only once and runs before loop()
  This is where you want to do your initial setup.  
*/
void setup() {
  // start the serial library:
  Serial.begin(9600);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  
  pinMode(RED,   OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE,  OUTPUT);
  
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
  
  printIpAddress();
  
  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    Serial.println("connected");
    client.println("GET / HTTP/1.0");
    client.println();

  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
    for(;;)
      ;
  }
}

void loop() {
  getColorFromServer();
  
  lightsOff();
  lightOn(pickLight(currentColor));
}

void connectToServer() {
  // attempt to connect, and wait a millisecond:
  Serial.println("connecting to server...");
  if (client.connect(server, port)) {
    Serial.println("making HTTP request...");
    
    // make HTTP GET request to the server:
    client.println("GET / HTTP/1.1");
    client.println("HOST: mwrc-ci.heroku.com");
    client.println();
  }
  // note the time of this connect attempt:
  lastAttemptTime = millis();
}

void getColorFromServer(){
   if (client.connected()) {
    if (client.available()) {
      // read incoming bytes:
      char inChar = client.read();

      // add incoming byte to end of line:
      currentLine += inChar; 

      // if you get a newline, clear the line:
      if (inChar == '\n') {
        currentLine = "";
      } 
      // we're looking for a line that begins with "color:"
      if ( currentLine.startsWith("<color>")) {
        readingColor = true;
        color += inChar;
      }
      if (currentLine.endsWith("</color>")) {
        readingColor = false;
        currentColor = color.substring(color.indexOf('>') + 1, color.indexOf('<'));
        //String color2 = color.substring(color.indexOf('>') + 1, color.indexOf('<'));
        //Serial.print("color: ");
        //Serial.println(color);
        Serial.print("currentColor: ");
        Serial.println(currentColor);
        // close the connection to the server:
        client.stop();
        color = "";
        currentLine = "";
      }
    }   
  }
  else if (millis() - lastAttemptTime > requestInterval) {
    // if you're not connected, and 10 seconds have passed since
    // your last connection, then attempt to connect again:
    connectToServer();
  }
 
}

void printIpAddress() {
  Serial.println("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
}

void blinkLight(int light) {
  
}

int pickLight(String lightName) {
  int result = RED;
  if (lightName.equals("blue")) {
    result = BLUE;
  }
  else {
      if (lightName.equals("green")) {
        result = GREEN;
      }
      else {
          if (lightName.equals("red")) {
            result = RED;  
          }
          else {
            result = RED;
          }
      }
  }

  return result;
}

void lightOn(int light) {
  digitalWrite(light, HIGH);
}

void lightsOn() {
 digitalWrite(RED,   HIGH);
 digitalWrite(GREEN, HIGH);
 digitalWrite(BLUE,  HIGH);
}

void lightOff(int light) {
  digitalWrite(light, LOW);
}

void lightsOff() {
 digitalWrite(RED,   LOW);
 digitalWrite(GREEN, LOW);
 digitalWrite(BLUE,  LOW);
}
