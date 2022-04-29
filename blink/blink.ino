#define LED 13
const int BUTTON = 2;
#define ECHO 3
#define TRIG 4

//#define TX 14
//#define RX 15

int value = 0;
long duration;
int distance;

int ledToggle = 0;
unsigned long currentTime = 0;
unsigned long lastTime = 0;


extern "C" {
  // function prototypes
  void start();
  void on();
  void off();
  void isr();
}

void setup() {
  // put your setup code here, to run once:
 start();
 //pinMode(LED, OUTPUT);
 pinMode(BUTTON, INPUT);
 //pinMode(TRIG, OUTPUT);
 //pinMode(ECHO, INPUT);
 //pinMode(TX, OUTPUT);
 //pinMode(RX, INPUT);
 Serial.begin(9600);
 //Serial2.begin(9600);
 Serial3.begin(9600);
 attachInterrupt(digitalPinToInterrupt(BUTTON),button_ISR, CHANGE);
 //attachInterrupt(digitalPinToInterrupt(RX),receive_ISR, CHANGE); //cannot interrupt on serial pins
}

void receive_ISR(){
  ledToggle = !ledToggle;
  if(ledToggle == 1){
    on();
  }
  else{
    off();
  }
}

void button_ISR(){
  
  //digitalWrite(TX, 1);
  //Serial3.print(5,DEC);
  //Serial.println("transmitted 5");
  //digitalWrite(TX,0);
  /*currentTime = millis();
  if(currentTime - lastTime > 250){
    ledToggle = !ledToggle;
    Serial.print(currentTime - lastTime);
    Serial.print(' ');
    Serial.print(ledToggle);
    if(ledToggle == 1){
      on();
      Serial.println(" On");
    }
    else{
      off();
      Serial.println(" Off");
    }
    lastTime = currentTime;
  }*/
}

void loop() {
  //value = 4;
  //Serial.println(value ,DEC);
  //delay(2500);
  
  currentTime = millis();
  int delayCurrent = currentTime - lastTime;
  //if(delayCurrent >= 1000){
    if(Serial3.available() > 0 ){
      int value = Serial3.parseInt();
      Serial.println(value);
      if(value > 0){
        lastTime = currentTime;
        //Serial.println("received: " + value);
        on();
        //delay(1000);
        /*do{
          (void)Serial3.read();
        }
        while(Serial3.available() != 0);*/
        //delay(1000);
      }
      else{
        Serial.println("off 2");
        //delay(10);
        off();
      }
    }
    else if (Serial3.available() <= 0){
  Serial.print("time last is: " );
  Serial.println(delayCurrent);
      Serial.println("off");
      //delay(10);
      off();
    }
  //}
  /*do{
      (void)Serial3.read();
    }
  while(Serial3.available() != 0);*/
  //delay(100);
  //digitalWrite(LED, 1);
  //delay(5000);
  //digitalWrite(LED,0);
  //delay(2000);
/*
  digitalWrite(TRIG,0);
  delayMicroseconds(2);
  digitalWrite(TRIG,1);
  delayMicroseconds(10);
  digitalWrite(TRIG,0);
  duration = pulseIn(ECHO, 1);
  distance = duration * 0.034 / 2;
  //Serial.println(distance);
  */
  //Serial.println(buttonState);
  /*if(distance <= 30){
    on();
  }
  else{
    off();
  }*/
  //delay(100); 
}
