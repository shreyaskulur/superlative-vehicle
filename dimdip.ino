int ut = 550, lt = 450;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);
}

long prev_dip  = 0;

void loop() {
  Serial.println(analogRead(0));
  
  if((analogRead(0) > ut))
  {
    digitalWrite(13, LOW);
    prev_dip = millis();
    Serial.println("        DIM");
  }
  else if(analogRead(0) < lt &&((millis() - prev_dip)>2000))
  {
    digitalWrite(13, HIGH);
    Serial.println("         Bright");
  }
}
