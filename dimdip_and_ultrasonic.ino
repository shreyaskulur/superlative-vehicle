int vcc = 2; //attach pin 2 to vcc
int trig = 3; // attach pin 3 to Trig
int echo = 4; //attach pin 4 to Echo
int gnd = 5; //attach pin 5 to GND
int ut = 550, lt = 450;

#include <Wire.h>

void setup() {

  pinMode (vcc, OUTPUT);
  pinMode (gnd, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  // initialize serial communication:
  Serial.begin(9600);

  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

long duration, inches;
long prev_dip  = 0;
long t = 0;
void loop()
{
  if ((millis() - t) > 100)
  {
    t = millis();
    digitalWrite(vcc, HIGH);
    // establish variables for duration of the ping,
    // and the distance result in inches and centimeters:

    // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(trig, OUTPUT);
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig, LOW);

    pinMode(echo, INPUT);
    duration = pulseIn(echo, HIGH);

    // convert the time into a distance
    inches = microsecondsToInches(duration);

    Serial.print("#");
    Serial.println(inches);
  }
  Serial.println(analogRead(0));

  if ((analogRead(0) > ut))
  {
    digitalWrite(13, LOW);
    prev_dip = millis();
    Serial.println("        DIM");
  }
  else if (analogRead(0) < lt && ((millis() - prev_dip) > 2000))
  {
    digitalWrite(13, HIGH);
    Serial.println("         Bright");
  }
}

void requestEvent() {
  Wire.write(inches); // respond with message
}

long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}
