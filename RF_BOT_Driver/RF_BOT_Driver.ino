#include <SPI.h>
#include "RF24.h"

#include <Wire.h>

#define lf_speed 5   //left front chip enable pin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
#define rf_speed 10  //right front chip enable pin
#define lb_speed 6   //left back chip enable pin
#define rb_speed 11  //right back chip enable pin
#define lmf 5   //left motor forward
#define lmb 6   //left motor back
#define rmf 9   //right motor forward
#define rmb 10   //left motor back

int l_speed = 255; // set left drive speed in the range 0 to 255
int r_speed = 255; // set right drive speed in the range 0 to 255
int l_dir; //1 => forward, -1 => backward
int r_dir;
int l_ip; //left input
int r_ip;
//turn left
void left ()
{
  digitalWrite(lmf, LOW);
  digitalWrite(lmb, HIGH);
  digitalWrite(rmf, HIGH);
  digitalWrite(rmb, LOW);
}

//turn right
void right ()
{
  digitalWrite(lmf, HIGH);
  digitalWrite(lmb, LOW);
  digitalWrite(rmf, LOW);
  digitalWrite(rmb, HIGH);
}

//move forward
void fwd ()
{
  digitalWrite(lmf, HIGH);
  digitalWrite(lmb, LOW);
  digitalWrite(rmf, HIGH);
  digitalWrite(rmb, LOW);
}

//move backwards
void rev ()
{
  digitalWrite(lmf, LOW);
  digitalWrite(lmb, HIGH);
  digitalWrite(rmf, LOW);
  digitalWrite(rmb, HIGH);
}

//stop motion
void stp ()
{
  digitalWrite(lmf, LOW);
  digitalWrite(lmb, LOW);
  digitalWrite(rmf, LOW);
  digitalWrite(rmb, LOW);
}

void left_drive(int spd, int dir)
{
  if (dir > 0)
  {
    analogWrite(lmf, spd);
    digitalWrite(lmb, LOW);
  }
  else if (dir < 0)
  {
    analogWrite(lmf, LOW);
    analogWrite(lmb, l_ip);
  }
  else
  {
    digitalWrite(lmf, LOW);
    digitalWrite(lmb, LOW);
  }
}

void right_drive(int spd, int dir)
{
  Serial.println(spd);
  if (dir > 0)
  {
    analogWrite(rmf, spd);
    digitalWrite(rmb, LOW);
  }
  else if (dir < 0)
  {
    analogWrite(rmf, LOW);
    analogWrite(rmb, r_ip);
  }
  else
  {
    digitalWrite(rmf, LOW);
    digitalWrite(rmb, LOW);
  }
}

void motor_init()
{
  pinMode(lmf , OUTPUT);
  pinMode(lmb , OUTPUT);
  pinMode(rmf , OUTPUT);
  pinMode(rmb , OUTPUT);
  analogWrite( lmf, 0);
  analogWrite( rmf, 0);
  analogWrite( lmb, 0);
  analogWrite( rmb, 0);
}

RF24 myRadio (7, 8);
struct package
{
  int lx;
  int ly;
  int rx;
  int ry;
  int l_key;
  int r_key;
};

byte addresses[][6] = {"0"};

typedef struct package Package;
Package data;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  delay(1000);
  motor_init();
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_LOW);
  myRadio.setDataRate( RF24_250KBPS ) ;
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}

double t = 0;
long distance = 100;

void loop()
{
  Wire.requestFrom(8, 1);    // request distance in long from slave device #8

  while (Wire.available()) { // slave may send less than requested
    distance = Wire.read(); // receive a byte as character
    Serial.println(distance);         // print the character
  }

  if ( myRadio.available())
  {
    while (myRadio.available())
    {
      myRadio.read( &data, sizeof(data) );
    }
    Serial.print("\nReceived Package:");
    Serial.print(data.lx);
    Serial.print("  ");
    Serial.print(data.ly);
    Serial.print("  ");
    Serial.print(data.rx);
    Serial.print("  ");
    Serial.print(data.ry);
    Serial.println("  ");
    if (data.ly > 600)
    {
      l_ip = (data.ly - 512) / 2;
      l_dir = -1;
    }
    else if (data.ly < 400)
    {
      l_ip = ((512 - data.ly) / 2) - 1;
      l_dir = 1;
    }
    else
    {
      l_ip = 0;
      l_dir = 0;
    }

    if (data.ry > 600)
    {
      r_ip = (data.ry - 512) / 2;
      r_dir = -1;
    }
    else if (data.ry < 400)
    {
      r_ip = ((512 - data.ry) / 2) - 1;
      r_dir = 1;
    }
    else
    {
      r_ip = 0;
      r_dir = 0;
    }
    t = millis();
  }
  
  if(distance > 10)
  {
    l_speed = l_ip;
    r_speed = r_ip;
    Serial.println("No breaking");
  }
  else if(distance > 8)
  {
    l_speed = l_ip/2;
    r_speed = r_ip/2;
    Serial.println("Breaking : stage 1");
  }
   else if(distance > 6)
  {
    l_speed = l_ip/4;
    r_speed = r_ip/4;
    Serial.println("Breaking : stage 2");
  }
  else 
  {
    stp();
    Serial.println("Breaking : STOP");
  }
  if (millis() > t + 1000)
    stp();
  else
  {
    left_drive(l_speed, l_dir);
    right_drive(r_speed, r_dir);
  }

  delay(1);
}
