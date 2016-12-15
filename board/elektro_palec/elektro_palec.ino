#include <Servo.h>
#include <SPI.h> // Required by Ethernet
#include <Ethernet.h>
#include <LiquidCrystal.h>


int servoPin = 12;
boolean logging;
Servo Servo1;
char authHeader[29] = "Authorization: Bearer DUPA.8";

byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0D, 0x1A, 0x14
};
// HARDCODED
IPAddress ip(192, 168, 1, 254);

EthernetServer server(80);

int ledPins[] = {9, 8, 7, 6, 5, 4, 3};

void setup() {
  Servo1.attach(servoPin);
  // set servo to init position
  Servo1.write(90);

  // flip to true when connected to a computer
  logging = false;

  // Open serial communications and wait for port to open:
  if(logging) {
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
  }

  Ethernet.begin(mac, ip); // connect

  delay(1000);
  server.begin();
  if(logging) {
    Serial.print("server is at ");
    Serial.println(Ethernet.localIP());
  }

  for (int i=0; i < sizeof(ledPins); i++){
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    if(logging) {
      Serial.println("New client");
    }
    // an http request ends with a blank line/
    boolean currentLineIsBlank = true;
    boolean headerAuthorized = false;
    int headerPointer = 0;
    for (int i=0; i < sizeof(ledPins); i++){
      digitalWrite(ledPins[i], HIGH);
      if(i < sizeof(ledPins) - 1) {
        delay(50);
      }
    }
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if(logging) {
          Serial.write(c);
        }

        // verify the auth header
        if (c == authHeader[headerPointer] && !headerAuthorized) {
          headerPointer++;
          Serial.println(headerPointer);
          if (headerPointer == 28) {
            headerAuthorized = true;
          }
        } else {
          headerPointer = 0;
        }

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          if (headerAuthorized) {
            // send a standard http response header
            client.println("HTTP/1.1 200 OK");
            moveFinger();
          } else {
            client.println("HTTP/1.1 403 UNAUTHORIZED");
          }
          client.println("Connection: close");
          client.println();
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // send all the things
    client.flush();
    // close the connection:
    client.stop();
    for (int i=0; i < sizeof(ledPins); i++){
      digitalWrite(ledPins[i], LOW);
      if(i < sizeof(ledPins) - 1) {
        delay(50);
      }
    }
    if(logging) {
      Serial.println("client disconnected");
    }
  }
}

void moveFinger() {
   Servo1.write(0); // down
   delay(250);
   Servo1.write(90); // up
}
