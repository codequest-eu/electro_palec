#include <Servo.h>
#include <SPI.h> // Required by Ethernet
#include <Ethernet.h>


int servoPin;
boolean logging;
Servo Servo1;

byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0D, 0x1A, 0x14
};
// HARDCODED
IPAddress ip(192, 168, 1, 254);

EthernetServer server(80);

void setup() {
  servoPin = 8;
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
}

// GET /jacek HTTP/1.1\n\r
// \n\r

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    if(logging) {
      Serial.println("new client");
    }
    // an http request ends with a blank line/
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if(logging) {
          Serial.write(c);
        }
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        // TODO sprawdz pan HEADER
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          moveFinger();
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
    if(logging) {
      Serial.println("client disconnected");
    }
  }
}

void moveFinger() {
   Servo1.write(0);
   delay(250);
   Servo1.write(90);
   delay(1000);
}
