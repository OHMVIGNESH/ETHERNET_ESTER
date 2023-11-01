#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
int ledr = 33;
int ledg = 32;
int ledb = 27;
char macip[6];
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress serverIP(8, 8, 8, 8); // Google's public DNS server
EthernetClient client;
char server[] = "www.google.com";

void setup() {
  Serial.begin(115200);
  delay(1000);
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(12,12);            // Start at top-left corner
  display.println(F("<-- NeT EXPRESS -->"));
  display.display();
  delay(5000);
  display.clearDisplay();
  display.setTextSize(1.5);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(12,12);            // Start at top-left corner
  display.println(F("CHECKING..."));
  display.display();
  delay(2000);
  Serial.println("Begin Ethernet");
  Ethernet.init(5); // MKR ETH Shield
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (true) {
      delay(1);
    }
  }
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    digitalWrite(ledr, HIGH);
    digitalWrite(ledg, LOW);
    digitalWrite(ledb, LOW);
    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0, 0);            // Start at top-left corner
    display.println("Failed to configure Ethernet using DHCP");
    display.display();
    delay(2000);
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      digitalWrite(ledr, LOW);
      digitalWrite(ledg, LOW);
      digitalWrite(ledb, HIGH);
      display.clearDisplay();
      display.setTextSize(1);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0, 0);            // Start at top-left corner
      display.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      display.display();
      delay(2000);
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
      digitalWrite(ledr, HIGH);
      digitalWrite(ledg, LOW);
      digitalWrite(ledb, LOW);
      display.clearDisplay();
      display.setTextSize(1);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0, 0);            // Start at top-left corner
      display.println(F("Ethernet cable is not connected."));
      display.display();
      delay(2000);
    }
 
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(sizeof(Ethernet.localIP()));
    digitalWrite(ledr, LOW);
    digitalWrite(ledg, LOW);
    digitalWrite(ledb, HIGH);
    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0, 0);            // Start at top-left corner  
    display.println(" DHCP LOCAL IP: ");
    display.setCursor(30, 20);
    display.println(Ethernet.localIP());
    display.display();
    delay(2000);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);











/*

 
  Serial.print("Local IP: ");
  Serial.println(Ethernet.localIP());
  Serial.println("Ethernet Successfully Initialized");

  // Now, let's add your first code's setup for connecting to a server.
  char server[] = "httpbin.org"; // Name address for the server (using DNS)

  if (client.connect(server, 80)) {
    Serial.println("Connected to httpbin.org!");
    // Make an HTTP request:
    client.println("GET /get HTTP/1.1");
    client.println("Host: httpbin.org");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("Failed to connect to httpbin.org!");
  }*/
}

void loop() {
  // You can add your first code's loop here.
  // The code below is for your second code's loop, which pings Google DNS.
 /* if (client.connect(serverIP, 443)) {
    Serial.println("Connected to Google DNS server!");
    client.stop();
  } else {
    Serial.println("Failed to connect to Google DNS server!");
  }

  delay(10000); // Wait for 5 seconds before pinging again*/
   int len = client.available();
  if (len > 0) {
    byte buffer[80];
    if (len > 80) len = 80;
    client.read(buffer, len);
    if (printWebData) {
      Serial.write(buffer, len); // show in the serial monitor (slows some boards)
    }
    byteCount = byteCount + len;
  }



  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    digitalWrite(ledr, LOW);
    digitalWrite(ledg, HIGH);
    digitalWrite(ledb, LOW);
    //display.println(F(Ethernet.localIP()));
   
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    //client.println("Connection: close");
    client.println();
    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(20, 0);            // Start at top-left corner
    display.println(F("<<<CONNECTED>>>"));
     display.setTextSize(0.5);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0, 10);            // Start at top-left corner
    display.println("PingIP:");
    display.setTextSize(0.5);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(40,10 );            // Start at top-left corner
    display.println(client.remoteIP());
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,22);            // Start at top-left corner  
    display.println("DHCPIP:");
    display.setCursor(40, 22);
    display.println(Ethernet.localIP());
    display.display();
    delay(2000);
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
    digitalWrite(ledr, HIGH);
    digitalWrite(ledg, LOW);
    digitalWrite(ledb, LOW);
    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0, 0);            // Start at top-left corner
    display.println(F("Connection failed"));
    display.display();
    delay(2000);

  }
  beginMicros = micros();
  if (!client.connected()) {
    digitalWrite(ledr, HIGH);
    digitalWrite(ledg, LOW);
    digitalWrite(ledb, LOW);
    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0, 0);            // Start at top-left corner
    display.println(F("Connection failed"));
    display.display();
    delay(2000);
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
}
