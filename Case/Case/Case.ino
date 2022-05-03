#include "DHT.h"
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32
#define OLED_RESET 4 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define REDLED 8
#define YELLOWLED 9 //define over in a asembler file later
#define GREENLED 10


void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  dht.begin();  
  display.display();
  delay(2000); 
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println("Setup done");
  display.display();
}

void loop() {
 float h = dht.readHumidity();
 float t = dht.readTemperature();
 display.clearDisplay();
 display.setCursor(0,0);
 display.println("DHT: ");
 if(!isnan(h)){
   display.print("  Huminity: ");
   display.println(h);
 }
 if(!isnan(t)){
   display.print("  Temperature ");
   display.println(t);
 }
 display.display();
}

void writeToDisplay(){
  
}
