#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
int ledr = 2;
int ledg = 3;
int ledb = 5;
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
char macip[6];
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

byte mac[] = {
  0x90, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

char server[] = "www.google.com";

IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);
EthernetClient client;

unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;

void setup() {

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  Serial.begin(9600);
  pinMode(ledr, OUTPUT);
  pinMode(ledg, OUTPUT);
  pinMode(ledb, OUTPUT);
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");
  Serial.println("Initialize Ethernet with DHCP:");
  digitalWrite(ledr, HIGH);
  digitalWrite(ledg, LOW);
  digitalWrite(ledb, LOW);
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
  Ethernet.begin(mac);

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
    // try to configure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
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

}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
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
