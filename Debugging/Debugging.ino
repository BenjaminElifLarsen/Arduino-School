#define ECHO 3
#define TRIG 4

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#include "Ticker.h";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void timeInterrupt();


volatile long duration;
volatile int distance;

volatile int flag = 0;

const int TIME_INTERRUPT_FLAG = 1;
const int BUTTON_INTERRUPT_FLAG = 2;
const int TIME_SET_FLAG = 4;

int INTERRUPT_TIME_MS = 2000;
int delayValue = 8;
volatile int interrupt_amount;
volatile bool ledFlag = 0;

const int MESSAGE_LENGTH = 6;

const char START_CHAR = '<';
const char END_CHAR = '>';

const int BUTTON = 2;
volatile int buttonPress = 0;

Ticker timer(timeInterrupt, INTERRUPT_TIME_MS, 0);

extern "C" {
  // function prototypes
  void start();
  void on();
  void off();
}
// This code was developed to use with the debuggin in the new version of ARDUINO IDE.

void setup() {
  start();
  Serial.begin(9600);
  flag = 0;
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUTTON, INPUT);
  timer.start();
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000); 
  display.clearDisplay();
  display.display();
  interrupt_amount = 0;
  attachInterrupt(digitalPinToInterrupt(BUTTON),button_ISR, CHANGE);
}

void loop() {
  delay(delayValue); //maybe count millis and if about delayValue call setTimeViaSerialPort();
  //Timer seems to be affected by the delay
  timer.update();
  if(flag != 0){
    Serial.print("flag value: ");
    Serial.println(flag);
    /*Serial.print("Time flag is set: ");
    Serial.println((flag & TIME_INTERRUPT_FLAG) == TIME_INTERRUPT_FLAG);
    Serial.print("Button flag is set: ");
    Serial.println(BUTTON_INTERRUPT_FLAG == (flag & BUTTON_INTERRUPT_FLAG) );*/
    if((flag & TIME_INTERRUPT_FLAG) == TIME_INTERRUPT_FLAG){
      //moved writeData out from here, since it also sets the button press display value.
    }
    if(BUTTON_INTERRUPT_FLAG == (flag & BUTTON_INTERRUPT_FLAG)){
      writeButtonData();
    }
    writeData();
    flag = 0;
  }
  setTimeViaSerialPort();
}

void writeData(){
  displayDistance(distance);
  Serial.println(distance);

}

void writeButtonData(){
  Serial.print("Button pressed: ");
  Serial.println(buttonPress);
}

void setTimeViaSerialPort(){
  if(Serial.available() > MESSAGE_LENGTH -1){
    bool hasStarted = false;
    bool hasEnded = false;
    bool hasError = false;
    int pos = 0;
    int dataSize = Serial.available();
    char bufferArray[MESSAGE_LENGTH-2];
    Serial.print("Data Size: ");
    Serial.println(dataSize);
    for(int i = 0; i < MESSAGE_LENGTH; i++){
      char value = Serial.read();
      Serial.print(value);
      if(0 == i){
        if(START_CHAR == value){
          hasStarted = true;
        }
        else{
          hasError = true;
          break;
        }
      }
      else if(END_CHAR == value){
          hasEnded = true;
      }
      else if(isDigit(value) && hasStarted && !hasEnded && !hasError){
        bufferArray[pos] = value;
        pos++;
      }
      else{
        hasError = true;
      }
    }
    Serial.println("");
    if(!hasError && hasStarted & hasEnded){
      Serial.print("New Timer: ");
      Serial.println(bufferArray);
      INTERRUPT_TIME_MS = atoi(bufferArray);
      if(INTERRUPT_TIME_MS < delayValue){
        INTERRUPT_TIME_MS = delayValue;
      }
      timer.interval(INTERRUPT_TIME_MS - delayValue);
      flag = flag | TIME_SET_FLAG;
    }
    else{
      Serial.print("Error: ");
      Serial.println(bufferArray);
    }
    do{
      (void)Serial.read();
    }while(Serial.available());
  }
}

void timeInterrupt(){
  getDistance();
  flag = flag | TIME_INTERRUPT_FLAG;
  interrupt_amount++;
  if(ledFlag){
    on();
  }else{
    off();
  }
  ledFlag = !ledFlag;
}

void button_ISR(){
  buttonPress++;
  flag = flag | BUTTON_INTERRUPT_FLAG;
}

int getDistance(){
  digitalWrite(TRIG,0);
  delayMicroseconds(2);
  digitalWrite(TRIG,1);
  delayMicroseconds(10);
  digitalWrite(TRIG,0);
  duration = pulseIn(ECHO, 1);
  distance = duration * 0.034 / 2;
  return distance;
}

void displayDistance(int distance){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2,1);
  display.println("CM:");
  display.setCursor(25, 1);
  display.println(distance);
  display.setCursor(2,12);
  display.println("I:");
  display.setCursor(25, 12);
  display.println(interrupt_amount);
  display.setCursor(2,23);
  display.println("MS:");
  display.setCursor(25, 23);
  display.println(INTERRUPT_TIME_MS);
  display.setCursor(60,1);
  display.println("F:");
  display.setCursor(74, 1);
  display.println(flag);
  display.setCursor(60,12);
  display.println("L:");
  display.setCursor(74, 12);
  display.println(ledFlag);
  display.setCursor(60,23);
  display.println("B:");
  display.setCursor(74, 23);
  display.println(buttonPress);
  display.display();
}
