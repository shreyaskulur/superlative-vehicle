#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (6, 7);
byte addresses[][6] = {"0"};

struct package
{
  int lx;
  int ly;
  int rx;
  int ry;
  int l_key;
  int r_key;
};


typedef struct package Package;
Package data;

int lx, ly, rx, ry;

void updateJoy()
{
  data.lx= analogRead(0);
  data.ly= analogRead(1);
  data.rx= analogRead(2);
  data.ry= analogRead(3);
  data.r_key = digitalRead(3);
  data.l_key = digitalRead(2);
  Serial.print("LX = ");
  Serial.print(data.lx);
  Serial.print("  LY = ");
  Serial.print(data.ly);
  Serial.print("  RX = ");
  Serial.print(data.rx);
  Serial.print("  RY = ");
  Serial.print(data.ry);
  Serial.print("  ");
  Serial.print(data.l_key);
  Serial.print("  ");
  Serial.println(data.r_key);
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  delay(1000);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
}

void loop()
{
  myRadio.write(&data, sizeof(data)); 
  updateJoy();
  
}
