#include <XBee.h>
#include <SoftwareSerial.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ssRX 9
#define ssTX 8

XBee xbee = XBee();

//SoftwareSerial xbee_ss(ssRX, ssTX);

ZBRxIoSampleResponse ioSamples = ZBRxIoSampleResponse();

ZBRxResponse rx = ZBRxResponse();


void setup() {
  
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.display();
  delay(2000); 
  display.clearDisplay();
  display.display();
  // put your setup code here, to run once:
  //xbee_ss.begin(9600);
  xbee.setSerial(Serial);
  write("Ready");
}

void loop() {
  // put your main code here, to run repeatedly:
  //write("1");
  xbee.readPacket();
  
  if(xbee.getResponse().isAvailable()){
    if (xbee.getResponse().getApiId() == 144)
    {
      xbee.getResponse().getZBRxResponse(rx);
      //write(String(rx.getRemoteAddress64().getLsb(), HEX));
      //display.println(rx.getRemoteAddress64().getLsb(), HEX);
      display.clearDisplay();
      display.setCursor(0,0);
      for(int i = 0; i < rx.getDataLength(); i++)
      {
        display.print((char)rx.getData(i));
      }
      display.display();
      //Serial.print("Expected I/O Sample, but got ");
      //Serial.print(xbee.getResponse().getApiId(), DEC);
    }  
    else{
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(xbee.getResponse().getApiId(), DEC);
    }
  }
  else if (xbee.getResponse().isError()) { 
      write(String(xbee.getResponse().getErrorCode()));
    //Serial.print("Error reading packet.  Error code: ");  
    //Serial.println(xbee.getResponse().getErrorCode());
  }
}

void write(String data){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2,1);
  display.println(data);
  display.display();
}
